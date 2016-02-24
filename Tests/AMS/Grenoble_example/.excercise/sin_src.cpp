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

#include "sin_src.h"

void sin_src::processing()
{
	outp=ampl*sin(2.0*M_PI*freq*get_time().to_seconds()); // + (rand()/double(RAND_MAX)-0.5)*0.3;
}

void sin_src::ac_processing()
{
	sca_ac(outp)=1.0;
}
