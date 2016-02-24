/*****************************************************************************
   (C) 2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden

 sin_src.h - description

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 16.02.2010

 SVN Version       :  $Revision$
 SVN last checkin  :  $Date$
 SVN checkin by    :  $Author$
 SVN Id            :  $Id$

 *****************************************************************************/

#include <systemc-ams.h>

SCA_TDF_MODULE(sin_src)
{
	sca_tdf::sca_out<double> outp;
	
	double ampl;
	double freq;
	
	
	void set_attributes()
	{
		outp.set_timestep(1.0,SC_US);
	}
	
	void processing();
	void ac_processing();
	
	
	SCA_CTOR(sin_src)
	{
		ampl=1.0;
		freq=1e3;
	}
};
