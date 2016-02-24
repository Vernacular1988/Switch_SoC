//pll.cpp
#include "pll.h"

// note this is a minor rewite of the pll in the tuv_ams bb library

/*****************************PLL***************************************/

    my_pll::my_pll(sc_core::sc_module_name n, double phc_gain, double lp_fc,
		   double vco_freq, double kvco, double vco_gain,
		   int vco_out_rate, int phc_out_rate)
    {
      phc_sub = new phc("phc_sub",phc_out_rate,phc_gain);
      phc_sub->in_ref(ref);
      phc_sub->in_vco(vcoo); //vcoo
      phc_sub->out(pco); //pco

      lp_sub = new lp("lp_sub",lp_fc);
      lp_sub->in(pco);  //pco
      lp_sub->out(lpo); //lpo  output 2

      vco_sub = new a_vco("vco_sub",vco_freq,vco_out_rate,kvco,vco_gain);
      vco_sub->in(lpo);  // lpo
      vco_sub->out(vcoo); // vcoo output 1
      vco_sub->out.set_delay(1);

    }

