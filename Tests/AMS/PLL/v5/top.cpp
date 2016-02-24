//top.cpp
#include "top.h"

// constructor
top::top(sc_core::sc_module_name n,
	 double s_freq,
	 double m_freq,
	 double s_amp,
	 double s_ph,
	 double s_offset,
	 int s_rate,
	 double phc_gain,
	 double lp_fc,
	 double vco_freq,
	 double kvco,
	 double vco_gain
	 )
{

//instantiate models
    sin1 = new sine("sin1", s_freq, s_amp, s_ph, s_offset, false, false, s_rate);
    sin1->out(source);
    sin1->set_timestep(10.0, SC_US);

    pll1 = new my_pll("pll1", phc_gain, lp_fc, vco_freq, kvco, vco_gain);
    pll1->ref(source);
    pll1->vcoo(vco_out_delay);
    pll1->lpo(lpf_out);

}
