/*****************************************************************************
  (C) 2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden

  prefi.h - description

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

  Created on: 16.02.2010

   SVN Version       :  $Revision$
   SVN last checkin  :  $Date$
   SVN checkin by    :  $Author$
   SVN Id            :  $Id$

 *****************************************************************************/
#ifndef PREFI_H_
#define PREFI_H_

#include <systemc-ams.h>

SCA_TDF_MODULE(prefi)
{
  sca_tdf::sca_in<double> inp;
  sca_tdf::sca_out<double> outp;
		
  //control / DE signal from SystemC
  //(connected to sc_signal<bool>)
  sca_tdf::sc_in<bool> ctrl;

  double fc0, fc1, v_max;

  //filter equation objects
  sca_tdf::sca_ltf_nd ltf_0, ltf_1;
  sca_util::sca_vector<double> a0,a1,b;
  sca_util::sca_vector<double> s;

	
  void initialize()
    {
      const double r2pi = M_1_PI * 0.5;  // reciprical of 2 * PI
      b(0) = 1.0;
      a0(0)= 1.0;
      a0(1)= r2pi/fc0;  // 1/(2*PI*F)
      a1(0)= 1.0;
      a1(1) = r2pi/fc1;
    }

  void processing()
    {
      double tmp; //high or low cut-off freq.
      // if(ctrl) 
	tmp = ltf_1(b,a1,s,inp);
	// else tmp = ltf_0(b,a0,s,inp);

      //output value limitation
      if (tmp > v_max) tmp = v_max; 
      else if (tmp < -v_max) tmp = -v_max;
      outp.write(tmp);
    }	

  SCA_CTOR(prefi)
    { 
      //default parameter values
      fc0 = 1.0e3; 
      fc1 = 1.0e5;	// note gtkwave resizes ampl of waveforms 
      v_max = 1.0;
    }
};

#endif


