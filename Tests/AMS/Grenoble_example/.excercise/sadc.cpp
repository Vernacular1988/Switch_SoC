/*****************************************************************************
   (C) 2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden

 sadc.cpp - description

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 16.02.2010

 SVN Version       :  $Revision$
 SVN last checkin  :  $Date$
 SVN checkin by    :  $Author$
 SVN Id            :  $Id$

 *****************************************************************************/

#include "sadc.h"

void sadc::processing()
{
	double s5 = integ2;
	double feedback = (s5 >= 0.0) ? 1.0 : -1.0;

	bool output = (s5 >= 0.0) ? true : false;
	outp.write(output);

	double s1 = inp.read() - feedback;

	double k1 = 0.5 * s1;
	double s2 = k1 + integ1;

	double k3 = 1.0 * feedback;
	double s3 = 0.5 * (integ1 - k3);

	double s4 = s3 + integ2;

	integ2 = s2;
	integ1 = s4;
}


void sadc::ac_processing()
{
	sca_ac(outp) = 0.25 / (sca_ac_z(2) - 1.5 * sca_ac_z() + 0.75) * sca_ac(inp);
}
