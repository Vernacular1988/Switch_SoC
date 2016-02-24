/*****************************************************************************
  (C) 2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden

  comb_filter.h - description

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

  Created on: 16.02.2010

   SVN Version       :  $Revision$
   SVN last checkin  :  $Date$
   SVN checkin by    :  $Author$
   SVN Id            :  $Id$

 *****************************************************************************/
#ifndef COMB_FILTER_H_
#define COMB_FILTER_H_

#include<systemc-ams>

SCA_TDF_MODULE(comb_filter)
{
	static const int WL=19;

	sca_tdf::sca_in<bool>                inp;
	sca_tdf::sca_out<sc_dt::sc_int<WL> > outp;

	????????

};



#endif /* COMB_FILTER_H_ */
