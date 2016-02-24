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

	void set_attributes()
	{
		inp.set_rate(64);
		outp.set_rate(1);

		out_int1.set_rate(64);
		out_int2.set_rate(64);
		out_int3.set_rate(64);
	}

	void ac_processing();
	void processing();

	SCA_CTOR(comb_filter);

	void trace_internals(sca_util::sca_trace_file*);

private:

	sc_dt::sc_int<WL> 	z1;
	sc_dt::sc_int<WL> 	z2;
	sc_dt::sc_int<WL> 	z3;
	sc_dt::sc_int<WL> 	z4;
	sc_dt::sc_int<WL> 	z5;
	sc_dt::sc_int<WL> 	z6;

	sca_tdf::sca_trace_variable<sc_dt::sc_int<WL> > out_int1;
	sca_tdf::sca_trace_variable<sc_dt::sc_int<WL> > out_int2;
	sca_tdf::sca_trace_variable<sc_dt::sc_int<WL> > out_int3;

	sca_tdf::sca_trace_variable<sc_dt::sc_int<WL> > out_diff1;
	sca_tdf::sca_trace_variable<sc_dt::sc_int<WL> > out_diff2;

	bool internals_traced;

};



#endif /* COMB_FILTER_H_ */
