//-------------------------------------------------------------------------------------------
// Copyright (c) 2008-2010, Institute of Computer Technology, University of Technology Vienna
// Authors:
//   Christoph Grimm, Jan Haase, Markus Damm, Jiong Ou,  
//   Joseph Wenninger, Peter Brunmayr, Muhammad Farooq
// Contact:
//   tuv-ams-lib@ict.tuwien.ac.at
// For the license please read LICENSE.txt
//-------------------------------------------------------------------------------------------

// Only tested for 8 kHz sampling rate, 16 bit samples, 1 channel

#ifndef WAVEREAD_H
#define WAVEREAD_H

#include <fstream>

class WaveRead
{
public:
	WaveRead(char *pFile);

	bool IsOpen();

	int ReadRIFFChunk();	//returns length of the file in byte
	int ReadFormatChunk();	//returns bytes per samples
	int ReadFactChunk();
	int ReadDataChunk();	//returns nr of samples in file

	unsigned int GetNrOfChannels();
	unsigned int GetFormatTag();
	unsigned int GetSampleRate();

	unsigned int CharToUInt(unsigned char *pData);
	unsigned short CharToUShort(unsigned char *pData);

	unsigned int ReadUInt();
	unsigned short ReadUShort();

	int GetSamples(unsigned char* pSamples, unsigned int NrOfBytes);

	~WaveRead();

private:
	std::ifstream fp;
	unsigned int FileLength;
	unsigned short FormatTag;
	unsigned short Channels;
	unsigned int SampleRate;
	unsigned int BytesPerSeconds;
	unsigned short BytesPerSample;
	unsigned int NrOfSamples;
	unsigned int BlockAlign;
};

#endif
