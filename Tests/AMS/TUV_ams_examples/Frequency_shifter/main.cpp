//-------------------------------------------------------------------------------------------
// Copyright (c) 2008-2010, Institute of Computer Technology, University of Technology Vienna
// Authors:
//   Christoph Grimm, Jan Haase, Markus Damm, Jiong Ou,  
//   Joseph Wenninger, Peter Brunmayr, Muhammad Farooq
// Contact:
//   tuv-ams-lib@ict.tuwien.ac.at
// For the license please read LICENSE.txt
//-------------------------------------------------------------------------------------------

#include "systemc-ams.h"

#include "WaveRead.h"
#include "WaveWrite.h"

#include <deque>
#include <iomanip>
#include "math.h"

#include "tuv_ams_library/tuv_ams_library.h"

using namespace sca_tdf;
using namespace TUV_ams_lib::bb;

#define PI 3.1415926535897932

//calculation of the hamming window
double hamming(int N, int i)
{
	return (0.54 - 0.46*cos(2*PI*double(i)/(double(N)-1)));
}

//==================================================================================
SCA_TDF_MODULE(Input)
{
	sca_out<double> out;

	//test wave file has to have the following format:
	// 16 bit per sample
	// mono
	// 8 kHz sampling rate
	Input(sc_module_name nm, char *pFile):mWave(pFile)
	{
		//read header of wave file
		int FileLength = mWave.ReadRIFFChunk();
		cout<<"Length of file: "<<FileLength<<endl;
		int BytePerSample = mWave.ReadFormatChunk();
		cout<<"Bytes per sample: "<<BytePerSample<<endl;
		int NrOfSamples = mWave.ReadDataChunk();
		cout<<"Number of samples: "<<NrOfSamples<<endl;
	}

	//reads a new value from the wave file and writes it to the channel
	void processing()
	{
		unsigned char Data[2];

		//read two bytes from wave file
		int NrOfSamples = mWave.GetSamples(Data,2);
		//at the end of the file, stop the simulation
		if (NrOfSamples < 1)
			sc_stop();

		//convert single bytes to one double value in the range of [-1;1[
		double Value = double(short(int(Data[0])<<8) | (Data[1])) / pow(2.0, 15.0);

		//write new value to channel
		out.write(Value);
	}

	WaveRead mWave;
};

//==================================================================================
template<int N, int Overlap>
SCA_TDF_MODULE(S2P_Overlap)
{
	sca_in<double> in;
	sca_out<double> out_real[N];
	sca_out<double> out_imag[N];

	SCA_CTOR(S2P_Overlap)
	{}

	void initialize()
	{
		//initialize the buffer
		for (int i=0; i<N; i++)
		{
			mBuffer.push_back(0);
		}
	}

	void set_attributes()
	{
		in.set_rate(N-Overlap);
	}

	void processing()
	{
		for (int i=0; i<(N-Overlap); i++)
		{
			//add an element at the end of the list
			mBuffer.push_back(in.read(i));
			//remove an element at the beginning of the list
			mBuffer.pop_front();
		}

		//write current buffer values to 1024 outputs
		for (int i=0; i<N; i++)
		{
			//apply hamming window
			out_real[i].write(mBuffer[i]*hamming(N,i));
			out_imag[i].write(0);
		}
	}

	std::deque<double> mBuffer;
};

//==================================================================================
template<int N>
SCA_TDF_MODULE(FrequShifter)
{
	sca_in<double> in_real[N];
	sca_in<double> in_imag[N];
	sca_out<double> out_real[N];
	sca_out<double> out_imag[N];

	FrequShifter(sc_module_name nm, int Shift):mShift(Shift)
	{}

	void processing()
	{
		//initialize all ports with 0
		for (int i=0; i<N; i++)
		{
			out_real[i] = 0;
			out_imag[i] = 0;
		}

		for (int i=0; i<N/2; i++)
		{
			//shift lower half of spectrum & insert 0
			if (((i - mShift) >= 0) && ((i + mShift) < (N/2)))
			{
				out_real[i-mShift] = in_real[i].read();
				out_imag[i-mShift] = in_imag[i].read();
			}
		}

		for (int i=N/2; i<N; i++)
		{
			//shift higher half o spectrum & insert 0
			if (((i - mShift) >= (N/2)) && ((i + mShift) < N))
			{
				out_real[i+mShift] = in_real[i].read();
				out_imag[i+mShift] = in_imag[i].read();
			}
		}
	}

	int mShift;
};


//==================================================================================
template<int N, int Overlap>
SCA_TDF_MODULE(P2S_Overlap)
{
	sca_in<double> in_real[N];
	sca_in<double> in_imag[N];
	sca_out<double> out;

	SCA_CTOR(P2S_Overlap)
	{}

	void initialize()
	{
		//initialize the buffer
		for (int i=0; i<N; i++)
		{
			mBuffer.push_back(0);
		}
	}

	void set_attributes()
	{
		out.set_rate(N-Overlap);
	}

	void processing()
	{
		//overlap and add method
		for (int i=0; i<N; i++)
		{
			mBuffer[i] = mBuffer[i] + in_real[i].read();
		}

		for (int i=0; i<(N-Overlap); i++)
		{
			out.write(mBuffer.front(), i);
			mBuffer.pop_front();
			mBuffer.push_back(0);
		}
	}

	std::deque<double> mBuffer;
};

//==================================================================================
SCA_TDF_MODULE(Output)
{
	sca_in<double> in;

	Output(sc_module_name nm, char *pFile):mWave(pFile)
	{
		if (!mWave.IsOpen())
		{
			cout<<"ERROR: Output file could not be opened or created."<<endl;
			cout<<"       Maybe it is currently in use."<<endl;
		}
	}

	void initialize()
	{
		//0x1 = PCM format, 1 channel, 8 kHz sampling rate, 16 bit per sample
		mWave.WriteHeader(0x1, 1, 8000, 16);
	}

	void processing()
	{
		//conversion to integer
		int InputValue = int(in.read()*pow(2.0, 15.0)*0.5);

		unsigned char Bytes[2];
		//lower byte
		Bytes[0] = char(InputValue & 0xFF);
		InputValue = InputValue >> 8;
		//higher byte
		Bytes[1] = char(InputValue & 0xFF);

		//write two bytes to the wave file
		mWave.PutSamples(Bytes, 2);
	}

	~Output()
	{
		mWave.FinishWaveWrite();
	}

	WaveWrite mWave;
};

//==================================================================================

int sc_main(int argc, char* argv[]) {

	int Shift;
	char* pInputFile;
	char* pOutputFile;


	//N has to be a power of two
	const int N = 1024;
	//has to be in the interval [0;N]
	const int Overlap = N/2;

	cout<<"========================================================"<<endl;

	//has to be in the interval [N/2;-N/2]
	if (argc > 1)
	{
		cout<<"Shift set to: "<<argv[1]<<endl;
		Shift = atoi(argv[1]);
	}
	else
		//default value*/
		Shift = 2;//0;

	//input file has to have the following format:
	// 8 kHz sampling frequency, 16 bit samples, 1 channel.
	if (argc > 2)
	{
		cout<<"Input file: "<<argv[2]<<endl;
		pInputFile = argv[2];
	}
	else
	{
		//default value*/
		cout<<"Input file: low.wav"<<endl;
		pInputFile = "low.wav";
	}
	cout<<"The input file has to have the following format:"<<endl;
	cout<<"1 channel, 8 kHz sampling rate, 16 bit samples"<<endl;

	//output file
	if (argc > 3)
	{
		cout<<"Output file: "<<argv[3]<<endl;
		pOutputFile = argv[3];
	}
	else
	{
		//default value
		cout<<"Output file: Output.wav"<<endl;
		pOutputFile = "Output.wav";
	}

	cout<<"========================================================"<<endl;

	sca_signal<double> sig_input, sig_output;

	sca_signal<double> in_time_real[N];
	sca_signal<double> in_time_imag[N];
	sca_signal<double> in_freq_real[N];
	sca_signal<double> in_freq_imag[N];

	sca_signal<double> out_freq_real[N];
	sca_signal<double> out_freq_imag[N];
	sca_signal<double> out2_freq_real[N];
	sca_signal<double> out2_freq_imag[N];
	sca_signal<double> out_real[N];
	sca_signal<double> out_imag[N];

	Input ReadWaveFile("Input", pInputFile);
	ReadWaveFile.out(sig_input);

	S2P_Overlap<N,Overlap> serial_2_parallel("S2P");
	serial_2_parallel.in(sig_input);

	fft_ifft<N> FFT("FFT", "FFT");

	FrequShifter<N> PS("PS", Shift);

	fft_ifft<N> IFFT("IFFT", "IFFT");

	P2S_Overlap<N,Overlap> parallel_2_serial("P2S");

	parallel_2_serial.out(sig_output);

	Output WriteWaveFile("Output", pOutputFile);
	WriteWaveFile.in(sig_output);

	for (int i=0; i<N; i++)
	{
		serial_2_parallel.out_real[i](in_time_real[i]);
		serial_2_parallel.out_imag[i](in_time_imag[i]);

		FFT.in_real[i](in_time_real[i]);
		FFT.in_imag[i](in_time_imag[i]);
		FFT.out_real[i](in_freq_real[i]);
		FFT.out_imag[i](in_freq_imag[i]);

		PS.in_real[i](in_freq_real[i]);
		PS.in_imag[i](in_freq_imag[i]);
		PS.out_real[i](out_freq_real[i]);
		PS.out_imag[i](out_freq_imag[i]);

		IFFT.in_real[i](out_freq_real[i]);
		IFFT.in_imag[i](out_freq_imag[i]);
		IFFT.out_real[i](out_real[i]);
		IFFT.out_imag[i](out_imag[i]);

		parallel_2_serial.in_real[i](out_real[i]);
		parallel_2_serial.in_imag[i](out_imag[i]);
	}

	ReadWaveFile.out.set_timestep(0.01, SC_US);

	//start simulation. it is stopped by Input module.
	sc_start();

	return 0;
}
