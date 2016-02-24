/*****************************************************************************
   (C) 2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden

 sadc.h - description

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 16.02.2010

 SVN Version       :  $Revision$
 SVN last checkin  :  $Date$
 SVN checkin by    :  $Author$
 SVN Id            :  $Id$

 *****************************************************************************/


#ifndef SADC_H_
#define SADC_H_

#include<systemc-ams.h>

SCA_TDF_MODULE(sadc)
{
	sca_tdf::sca_in<double> inp;
	sca_tdf::sca_out<bool>  outp;

	void processing();
	void ac_processing();

	SCA_CTOR(sadc)
	{
		integ1=0.0;
		integ2=0.0;
	}

private:

	double integ1, integ2;

};


#endif /* SADC_H_ */
