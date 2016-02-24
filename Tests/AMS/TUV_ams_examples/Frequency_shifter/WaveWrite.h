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

#ifndef WAVEWRITE_H
#define WAVEWRITE_H

#include <fstream>

class WaveWrite
{
public:
	//------------------------------------------------------------------
	WaveWrite(char *pFile);

	bool IsOpen();

	//------------------------------------------------------------------
	bool WriteHeader(unsigned short DataFormat, unsigned short NrOfChannels, \
		unsigned int SampleRate, unsigned short BitsPerSample);
	unsigned int PutSamples(unsigned char* pSamples, unsigned int NrOfBytes);
	bool FinishWaveWrite();

	unsigned int WriteUInt(unsigned int Data);
	unsigned int WriteUShort(unsigned short Data);

	//------------------------------------------------------------------
	~WaveWrite();

private:
	std::fstream fp;
	unsigned int FileLength;
	unsigned short FormatTag;
	unsigned short Channels;
	unsigned int SampleRate;
	unsigned int BytesPerSeconds;
	unsigned short BytesPerSample;
	unsigned int BlockAlign;
	unsigned int DataLength;

	long PosFactLength;
	long PosFMTLength;
};

#endif
