// osc.cpp
#include "osc.h"

//constructor
osc::osc(sc_core::sc_module_name n,
	 double phc_gain,
	 double lp_fc,
	 double vco_freq,
	 double kvco,
	 double vco_gain
	 ) 
{
  //instantiate models

  mul1 = new mul("mul1");
  mul1->in1(vco_ref1);
  mul1->in2(w1);
  mul1->out(mul1_out);

  mul2 = new mul("mul2");
  mul2->in1(vco_ref2);
  mul2->in2(w2);
  mul2->out(mul2_out);

  adder = new add("adder");
  adder->in1(mul1_out);
  adder->in2(mul2_out);
  adder->out(adder_out);
  
  pll1 = new my_pll("pll1", phc_gain, lp_fc, vco_freq, kvco, vco_gain);
  pll1->ref(adder_out);
  pll1->vcod(vco_out);
  pll1->lpo(lpf_out);

}
