/*****************************************************************************
   (C) 2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden

 sin_src_tb.cpp - description

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 16.02.2010

 SVN Version       :  $Revision$
 SVN last checkin  :  $Date$
 SVN checkin by    :  $Author$
 SVN Id            :  $Id$

 *****************************************************************************/

#include "sin_src.h"

int sc_main(int argn,char* argc[])
{
	sca_tdf::sca_signal<double> s1;
	
	sin_src sin1("sin1");
	  sin1.outp(s1);
	
	sca_trace_file* tf=sca_create_tabular_trace_file("sin_tb.dat");
	  sca_trace(tf,s1,"s1");
	
	sc_start(10.0,SC_MS);
	
	sca_close_tabular_trace_file(tf);
	
	tf=sca_create_tabular_trace_file("sin_ac_tb.dat");
	 sca_trace(tf,s1,"s1");
	   tf->set_mode(sca_ac_format(sca_util::SCA_AC_DB_DEG));
	
	
	sca_ac_start(1.0,1e6,1000,SCA_LOG);
	 
	
	return 0;
}
