/*****************************************************************************
   (C) 2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden

 comb_filter.cpp - description

 Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

 Created on: 16.02.2010

 SVN Version       :  $Revision$
 SVN last checkin  :  $Date$
 SVN checkin by    :  $Author$
 SVN Id            :  $Id$

 *****************************************************************************/

#include <systemc-ams.h>
#include "comb_filter.h"

comb_filter::comb_filter(sc_core::sc_module_name nm)
{
	z1 = 0;
	z2 = 0;
	z3 = 0;
	z4 = 0;
	z5 = 0;
	z6 = 0;

	internals_traced = false;
}

void comb_filter::ac_processing()
{
	double k = 64.0;
	double n = 3.0;

	sca_complex z = sca_ac_z(1, get_timestep());
	sca_complex h;

	h = pow((1.0 - pow(z, -k)) / (1.0 - 1.0 / z), n); // comb

	sca_ac(outp) = h * sca_ac(inp);

}

void comb_filter::processing()
{
	sc_int<WL> y, y1, y2, y3, y4, y5;

	//Integrators
	for (int i = 0; i < 64; i++)
	{
		y1 = inp.read(i) ? z1 + 1 : z1 - 1;
		z1 = y1;

		y2 = y1 + z2;
		z2 = y2;

		y3 = y2 + z3;
		z3 = y3;

		if (internals_traced)
		{
			out_int1.write(y1, i);
			out_int2.write(y2, i);
			out_int3.write(y3, i);
		}
	}

	//Differentiators
	y4 = y3 - z4;
	z4 = y3;

	y5 = y4 - z5;
	z5 = y4;

	y = y5 - z6;
	z6 = y5;

	if (internals_traced)
	{
		out_diff1 = y4;
		out_diff2 = y5;
	}

	outp.write(y);
}

void comb_filter::trace_internals(sca_util::sca_trace_file* tf)
{
	internals_traced = true;

	sca_util::sca_trace(tf, out_int1, name() + std::string(".out_int1"));
	sca_util::sca_trace(tf, out_int2, name() + std::string(".out_int2"));
	sca_util::sca_trace(tf, out_int3, name() + std::string(".out_int3"));

	sca_util::sca_trace(tf, out_diff1, name() + std::string(".out_diff1"));
	sca_util::sca_trace(tf, out_diff2, name() + std::string(".out_diff2"));
}

