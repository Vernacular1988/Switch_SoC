//-------------------------------------------------------------------------------------------
// Copyright (c) 2008-2010, Institute of Computer Technology, University of Technology Vienna
// Authors:
//   Christoph Grimm, Jan Haase, Markus Damm, Jiong Ou,  
//   Joseph Wenninger, Peter Brunmayr, Muhammad Farooq
// Contact:
//   tuv-ams-lib@ict.tuwien.ac.at
// For the license please read LICENSE.txt
//-------------------------------------------------------------------------------------------

#include "WaveWrite.h"

WaveWrite::WaveWrite(char *pFile)
{
	fp.open(pFile, std::fstream::binary | std::fstream::out);
}

bool WaveWrite::IsOpen()
{
	return fp.is_open();
}

unsigned int WaveWrite::WriteUInt(unsigned int Data)
{
	fp.put((char)((Data)       & 0xff));
	fp.put((char)((Data >>  8) & 0xff));
	fp.put((char)((Data >> 16) & 0xff));
	fp.put((char)((Data >> 24) & 0xff));

	return 4;
}

unsigned int WaveWrite::WriteUShort(unsigned short Data)
{
	fp.put((char)((Data)       & 0xff));
	fp.put((char)((Data >>  8) & 0xff));

	return 2;
}

bool WaveWrite::WriteHeader(unsigned short DataFormat, unsigned short NrOfChannels, \
		unsigned int SampleRate, unsigned short BitsPerSample)
{
	const char *pChunkID = "RIFF";
	const char *pRIFFType = "WAVE";
	const char *pFormatHeader = "fmt ";
	const char *pDataHeader = "data";
	const char *pFactHeader = "fact";

	FileLength = 44; //length of header
	FormatTag = DataFormat;
	Channels = NrOfChannels;
	SampleRate = SampleRate;
	BytesPerSample = BitsPerSample/8;
	BlockAlign = BytesPerSample*NrOfChannels;
	BytesPerSeconds = SampleRate*BlockAlign;
	DataLength = 0;

	// write RIFF header
	//++++++++++++++++++++++++++
	//RIFF
	for (int i=0; i<4; i++)
		fp.put(pChunkID[i]);

	//length field
	WriteUInt(1);

	//WAVE
	for (int i=0; i<4; i++)
		fp.put(pRIFFType[i]);

	// write fmt chunk
	//++++++++++++++++++++++++++
	//fmt
	for (int i=0; i<4; i++)
		fp.put(pFormatHeader[i]);

	//fmt header length
	WriteUInt(16);

	//sample data format
	WriteUShort(DataFormat);

	//nr of channels
	WriteUShort(NrOfChannels);

	//sample rate
	WriteUInt(SampleRate);

	//bytes/second
	WriteUInt(BytesPerSeconds);

	//block align
	WriteUShort(BlockAlign);

	//bits per sample
	WriteUShort(BitsPerSample);

	if (FormatTag == 6) //a-law
	{
		// write fact chunk
		//++++++++++++++++++++++++++
		//fact
		for (int i=0; i<4; i++)
			fp.put(pFactHeader[i]);

		//size of fact chunk
		WriteUInt(4);

		//chunk data = data length
		PosFactLength = fp.tellp();
		WriteUInt(0);
	}

	// write data chunk
	//++++++++++++++++++++++++++
	//data
	for (int i=0; i<4; i++)
		fp.put(pDataHeader[i]);

	PosFMTLength = fp.tellp();
	//length of data chunk
	WriteUInt(0);

	return true;
}


unsigned int WaveWrite::PutSamples(unsigned char* pSamples, unsigned int NrOfBytes)
{
	for (int i=0; i<NrOfBytes; i++)
		fp.put(pSamples[i]);

	DataLength += NrOfBytes;
	return DataLength;
}

bool WaveWrite::FinishWaveWrite()
{
	FileLength += DataLength;
	long x = fp.tellp();

	fp.seekp(PosFMTLength);
	WriteUInt(DataLength);

	//write length in riff header
	fp.seekp(4);
	WriteUInt(FileLength-8);

	//write length in data chunk
	fp.seekp(PosFMTLength);
	WriteUInt(DataLength);

	if (FormatTag == 6) //a-law
	{
		//write length in fact chunk
		fp.seekp(PosFactLength);
		WriteUInt(DataLength/Channels);
	}

	return true;
}

WaveWrite::~WaveWrite()
{
	fp.close();
}
