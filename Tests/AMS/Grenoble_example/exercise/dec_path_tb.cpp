/*****************************************************************************
   (C) 2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden

 dec_path_tb.cpp - description

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 16.02.2010

 SVN Version       :  $Revision$
 SVN last checkin  :  $Date$
 SVN checkin by    :  $Author$
 SVN Id            :  $Id$

 *****************************************************************************/

#include "sin_src.h"
#include "prefi.h"
#include "sadc.h"
#include "comb_filter.h"

int sc_main(int argn,char* argc[])
{
	sca_tdf::sca_signal<double> s1, s2;
	sca_tdf::sca_signal<bool> s3;
	sca_tdf::sca_signal<sc_int<19> > s4;
	sc_signal<bool> ctrl;
	
	sin_src sin1("sin1");
	  sin1.outp(s1);
	
	prefi prefi1("lp1");
	  prefi1.inp(s1);
	  prefi1.outp(s2);
	  prefi1.ctrl(ctrl);
	
	sadc adc1("adc1");
	  adc1.inp(s2);
	  adc1.outp(s3);
	
	comb_filter dec1("dec1");
	  dec1.inp(s3);
	  dec1.outp(s4);
	
	????????
	
	return 0;
}
