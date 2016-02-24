// pll.h

#ifndef PLL_H
#define PLL_H

#include <systemc-ams.h>
#include "tuv_ams_library/tuv_ams_library.h"
#include "a_delay.h"

using namespace TUV_ams_lib::bb;

SC_MODULE(my_pll) {

  sca_tdf::sca_in<double> ref;
  sca_tdf::sca_out<double> vcod, lpo;

 private:
  sca_tdf::sca_signal<double>  pco;
  sca_tdf::sca_signal<double>  vcoo;
  phc* phc_sub;
  lp*  lp_sub;
  a_vco* vco_sub;
  a_delay* vco_delay;
 
 public:
  my_pll(sc_core::sc_module_name n,double phc_gain,
	 double lp_fc,double vco_freq,double kvco,
	 double vco_gain,int vco_out_rate=1,int phc_out_rate=1);
};

#endif
