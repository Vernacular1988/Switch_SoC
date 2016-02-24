/*****************************************************************************
   (C) 2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden

 prefi.cpp - description

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 16.02.2010

 SVN Version       :  $Revision$
 SVN last checkin  :  $Date$
 SVN checkin by    :  $Author$
 SVN Id            :  $Id$

 *****************************************************************************/

#include "prefi.h"

void prefi::initialize()
{
	B(0) = 1.0;
	A1(0) = 1.0;
	A1(1) = 1.0/(2.0*M_PI*fc1);
	A2(0) = 1.0;
	A2(1) = 1.0/(2.0*M_PI*fc2);
}

void prefi::processing()
{
	if(ctrl.read())
	{
		outp=ltf1(B,A2,inp);
	}
	else
	{
		outp=ltf1(B,A1,inp);
	}
}


void prefi::ac_processing()
{
	if(ctrl.read())
	{
		sca_ac(outp)=sca_ac_ltf_nd(B,A2,sca_ac(inp));
	}
	else
	{
		sca_ac(outp)=sca_ac_ltf_nd(B,A1,sca_ac(inp));
	}
}




