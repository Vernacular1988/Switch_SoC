// pll.h

#ifndef PLL_H
#define PLL_H

#include <systemc-ams.h>
#include "tuv_ams_library/tuv_ams_library.h"

using namespace TUV_ams_lib::bb;


// sink of data 
SCA_TDF_MODULE(drain)
{
  sca_tdf::sca_in<double> in;

  SCA_CTOR(drain) {}
};

// note this is a minor rewrite of the pll in tuv_lib bb

/**************************PLL*************************/

SC_MODULE(my_pll) {
 public:
  sca_tdf::sca_in<double>   ref;
  sca_tdf::sca_out<double>  vcoo, lpo;

 private:
  sca_tdf::sca_signal<double>  pco;

  phc* phc_sub;
  lp*  lp_sub;
  a_vco* vco_sub;

  void set_attributes() {
    vco_sub->out.set_delay(1);
  }

 public:
  my_pll(sc_core::sc_module_name n,double phc_gain,double lp_fc,
      double vco_freq,double kvco,double vco_gain,int vco_out_rate=1,
      int phc_out_rate=1);
};


#endif
