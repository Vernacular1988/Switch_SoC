//-------------------------------------------------------------------------------------------
// Copyright (c) 2008-2010, Institute of Computer Technology, University of Technology Vienna
// Authors:
//   Christoph Grimm, Jan Haase, Markus Damm, Jiong Ou,  
//   Joseph Wenninger, Peter Brunmayr, Muhammad Farooq
// Contact:
//   tuv-ams-lib@ict.tuwien.ac.at
// For the license please read LICENSE.txt
//-------------------------------------------------------------------------------------------

// This file contains the main declararations

double freq_bit = 1000000.;     	                // Bit rate of the digital signal sourece, 1 bit per 1/freq_bit second; 

double freq_carrier = 1000000000.0;                 // Frequency of carrier wave in Hz
				
int data_rate=10;                                   // The parameter ensures the data_rate number of tokens per sine period on the output port of the transmitter. A value too low would result in an undesirable outcome regarding Nyquist's sampling theorem. 
