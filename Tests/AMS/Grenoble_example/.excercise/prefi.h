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
	
	sca_tdf::sc_in<bool>  ctrl;
	
	double fc1, fc2;
	
	void initialize();
	void processing();
	void ac_processing();
	
	SCA_CTOR(prefi)
	{
		fc1=1e3;
		fc2=10e3;
	}
		
	sca_vector<double> A1,A2,B,s;
	sca_tdf::sca_ltf_nd ltf1;
};
