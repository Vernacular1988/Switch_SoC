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

/*****************************************************************************/


#ifndef SADC_H_
#define SADC_H_

#include<systemc-ams.h>

SCA_TDF_MODULE(sadc)
{
  sca_tdf::sca_in<double> inp;
  sca_tdf::sca_out<bool>  outp;
	
  void initialize() {}
 
  void processing() {
    if(inp > 0.0) outp.write(1);
    else outp.write(0);
  }

  SCA_CTOR(sadc) {}


};


#endif /* SADC_H_ */
