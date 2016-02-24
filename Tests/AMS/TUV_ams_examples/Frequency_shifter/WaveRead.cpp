//-------------------------------------------------------------------------------------------
// Copyright (c) 2008-2010, Institute of Computer Technology, University of Technology Vienna
// Authors:
//   Christoph Grimm, Jan Haase, Markus Damm, Jiong Ou,  
//   Joseph Wenninger, Peter Brunmayr, Muhammad Farooq
// Contact:
//   tuv-ams-lib@ict.tuwien.ac.at
// For the license please read LICENSE.txt
//-------------------------------------------------------------------------------------------

#include "WaveRead.h"

WaveRead::WaveRead(char *pFile)
{
	fp.open(pFile, std::ios::binary | std::ios::in);
}

bool WaveRead::IsOpen()
{
	return fp.is_open();
}

int WaveRead::ReadRIFFChunk()
{
	char temp;

	//read chunkid
	const char *pChunkID = "RIFF";
	for (int i=0; i<4; i++)
	{
		fp.get(temp);
		if (temp != pChunkID[i])
			return -1;
	}

	//read length of file
	FileLength = ReadUInt();

	//read riff type
	const char *pRIFFType = "WAVE";
	for (int i=0; i<4; i++)
	{
		fp.get(temp);
		if (temp != pRIFFType[i])
			return -1;
	}

	return FileLength;
}

int WaveRead::ReadFormatChunk()
{
	char temp;

	//read chunkid
	const char *pFormatHeader = "fmt ";
	for (int i=0; i<4; i++)
	{
		fp.get(temp);
		if (temp != pFormatHeader[i])
			return -1;
	}

	int FMTLength = ReadUInt();
	//if (ReadUInt() != 16)
	if (FMTLength < 16)
		return -1;

	FormatTag = ReadUShort();
	//check wave file format - only pcm/uncompressed and a-law are permitted
	if ((FormatTag != 1) && (FormatTag != 6))
		return -1;

	Channels = ReadUShort();
	SampleRate = ReadUInt();
	ReadUInt();//read bytes/second = samplerate*block-align
	BlockAlign = ReadUShort();//channels*bits/sample/8
	BytesPerSample = ReadUShort()/8;

	//read extra format bytes
	FMTLength = FMTLength - 16;
	while (FMTLength >0)
	{
		fp.get(temp);
		FMTLength--;
	}

	return BytesPerSample;
}

int WaveRead::ReadDataChunk()
{
	//if a-law, then read fact chunk first
	if (FormatTag == 6)
		if (ReadFactChunk())
			return -1;

	char temp;
	const char *pDataHeader = "data";
	for (int i=0; i<4; i++)
	{
		fp.get(temp);
		if (temp != pDataHeader[i])
			return -1;
	}

	unsigned int DataLength = ReadUInt();
	NrOfSamples = DataLength/BytesPerSample;
	return NrOfSamples;
}

int WaveRead::ReadFactChunk()
{
	char temp;
	//read chunk id
	const char *pDataHeader = "fact";
	for (int i=0; i<4; i++)
	{
		fp.get(temp);
		if (temp != pDataHeader[i])
			return -1;
	}

	//read length of fact chunk
	//unsigned int FactLength = ReadUInt();
	ReadUInt();

	//read format dependent data of fact chunk
	//unsigned int lenght = ReadUInt();
	ReadUInt();

	return 0;
}

unsigned int WaveRead::GetNrOfChannels()
{
	return Channels;
}

unsigned int WaveRead::GetFormatTag()
{
	return FormatTag;
}

unsigned int WaveRead::GetSampleRate()
{
	return SampleRate;
}

unsigned int WaveRead::CharToUInt(unsigned char *pData)
{
	return (pData[0] << 24) | (pData[1] << 16) | (pData[2] << 8) | pData[3];
}

unsigned short WaveRead::CharToUShort(unsigned char *pData)
{
	return (pData[0] << 8) | pData[1];
}

unsigned int WaveRead::ReadUInt()
{
	unsigned char Buffer[4];
	for (int i=3; i>=0; i--)
		fp.get((char &)Buffer[i]);
	return CharToUInt(Buffer);
}

unsigned short WaveRead::ReadUShort()
{
	unsigned char Buffer[2];
	fp.get((char &)Buffer[1]);
	fp.get((char &)Buffer[0]);
	return CharToUShort(Buffer);
}

int WaveRead::GetSamples(unsigned char* pSamples, unsigned int NrOfBytes)
{
	//check if passed array is large enough
	if (NrOfBytes < BlockAlign)
		return -1;
	int i = 0;
	for (int j=0; j<Channels; j++)
	{
		switch(BytesPerSample)
		{
		case 1:
			fp.get((char &)pSamples[i++]);
			NrOfSamples--;
			break;
		case 2:
			fp.get((char &)pSamples[i+1]);
			fp.get((char &)pSamples[i]);
			i = i + 2;
			NrOfSamples--;
			break;
		case 3:
			fp.get((char &)pSamples[i+2]);
			fp.get((char &)pSamples[i+1]);
			fp.get((char &)pSamples[i]);
			i = i + 3;
			NrOfSamples--;
			break;
		}
	}
	return NrOfSamples;
}

WaveRead::~WaveRead()
{
	fp.close();
}
