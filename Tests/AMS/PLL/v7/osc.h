//osc.h
// hard coded for 3x3 for now, need busses 

#ifndef OSC_H
#define OSC_H

#include <systemc-ams.h>
#include "tuv_ams_library/tuv_ams_library.h"
#include "pll.h"

using namespace TUV_ams_lib::bb;

SC_MODULE(osc) {

  /********** defining ios *********** */
  sca_tdf::sca_in<double> vco_ref1, vco_ref2;
  sca_tdf::sca_in<double> w1, w2; 
  sca_tdf::sca_out<double> vco_out;
  sca_tdf::sca_out<double> lpf_out;

  /********** defining signals *********** */
 private:
  sca_tdf::sca_signal<double> mul1_out, mul2_out, adder_out;

  /*** Sub Blocks ****/
  my_pll *pll1;
  mul *mul1, *mul2;
  add *adder;
  
 public: // constructor
  osc(sc_core::sc_module_name n,
      double phc_gain = 100.0,	// phase comparitor gain
      double lp_fc = 10.0,	// low pass filter cut off freq
      double vco_freq = 1000.0, // natural frequency of vco
      double kvco = 100.0,	// control sensitivity for vco
      double vco_gain = 1.0	// amplitude gain of vco
      );	                        
};
 
#endif
