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

#include <systemc-ams.h>

SCA_TDF_MODULE(prefi)
{
	sca_tdf::sca_in<double> inp;
	sca_tdf::sca_out<double> outp;
	
	
	double fc1, fc2;
	
	????
	
	SCA_CTOR(prefi)
	{
		fc1=1e3;
		fc2=10e3;
	}
		
	????
};
