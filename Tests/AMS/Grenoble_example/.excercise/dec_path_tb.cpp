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
	
	sca_trace_file* tf=sca_create_tabular_trace_file("path_tb.dat");
	  sca_trace(tf,s1,"s1");
	  sca_trace(tf,s2,"s2");
	  sca_trace(tf,s3,"s3");
	  dec1.trace_internals(tf);
	  sca_trace(tf,s4,"s4");
	  sca_trace(tf,ctrl,"ctrl");
	
	ctrl.write(false);
	sc_start(5.0,SC_MS);
	
	tf->reopen("path_ac1_tb.dat");
	tf->set_mode(sca_ac_format(sca_util::SCA_AC_DB_DEG));
	
	sca_ac_start(1.0,1e6,1000,SCA_LOG);
	
	tf->reopen("path_tb.dat",std::ios::app);
	
	ctrl.write(true);
	sc_start(5.0,SC_MS);
	
	tf->reopen("path_ac2_tb.dat");
	sca_ac_start(1.0,1e6,1000,SCA_LOG);
	
	return 0;
}
