// pll.h

#ifndef PLL_H
#define PLL_H

#include <systemc-ams.h>
#include "tuv_ams_library/tuv_ams_library.h"

using namespace TUV_ams_lib::bb;

// analog delay for pll based on digitial delay in TUV BB

SCA_TDF_MODULE(a_delay) {

  sca_tdf::sca_in<double> inp;
  sca_tdf::sca_out<double> outp;

 private:
  int delay;
  double initial;
  void initialize();
  void set_attributes();
  void processing();

 public:
  a_delay::a_delay(sc_core::sc_module_name n, double _init=0.0, int _delay=1);
};

// SCA_TDF_MODULE(my_pll) {

//   sca_tdf::sca_in<double> ref, vcod;
//   sca_tdf::sca_out<double> vcoo, lpo;

//  private:
//   sca_tdf::sca_signal<double>  pco;
//   //sca_tdf::sca_signal<double>  vcod;

//   phc* phc_sub;
//   lp*  lp_sub;
//   a_vco* vco_sub;
//   //a_delay* vco_delay;
 
//  public:
//   my_pll(sc_core::sc_module_name n,double phc_gain,
//       double lp_fc,double vco_freq,double kvco,
//       double vco_gain,int vco_out_rate=1,int phc_out_rate=1);
//};

#endif
