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
//    sin1 = new sine("sin1", s_freq, s_amp, s_ph, s_offset, false, false, s_rate);
    //sin1->out(source);

    pll1 = new my_pll("pll1", phc_gain, lp_fc, vco_freq, kvco, vco_gain);
    pll1->ref(vco_out_3);
    pll1->vcod(vco_out_1);
    pll1->lpo(lpf_out1);

    pll2 = new my_pll("pll2", phc_gain, lp_fc, vco_freq+100, kvco, vco_gain);
    pll2->ref(vco_out_1);
    pll2->vcod(vco_out_2);
    pll2->lpo(lpf_out2);

    pll3 = new my_pll("pll3", phc_gain, lp_fc, vco_freq+100, kvco, vco_gain);
    pll3->ref(vco_out_2);
    pll3->vcod(vco_out_3);
    pll3->lpo(lpf_out3);


}
