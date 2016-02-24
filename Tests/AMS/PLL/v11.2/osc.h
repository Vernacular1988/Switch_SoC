//osc.h

#ifndef OSC_H
#define OSC_H

// array version 

#include "size.h"
#include <systemc-ams.h>
#include "tuv_ams_library/tuv_ams_library.h"
#include "pll.h"

using namespace TUV_ams_lib::bb;

SC_MODULE(osc) {

  /********** defining ios *********** */
  sca_tdf::sca_in<double> VCO_IN[NN];
  sca_tdf::sca_in<double> W[NN]; 
  sca_tdf::sca_out<double> VCO_OUT;
  sca_tdf::sca_out<double> LPF_OUT;

  /********** defining signals *********** */
 private:
  sca_tdf::sca_signal<double> MULA[NN], ADDER[NN-1];

  /*** Sub Blocks ****/
  my_pll *pll1;
  mul *mula[NN];
  add *adder[NN-1];
  
 public: // constructor
  osc(sc_core::sc_module_name n,
      double phc_gain = 100.0,	// phase comparitor gain
      double lp_fc = 10.0,	// low pass filter cut off freq
      double vco_freq = 1000.0, // natural frequency of vco
      double kvco = 100.0,	// control sensitivity for vco
      double vco_gain = 1.0,	// amplitude gain of vco
      double phase = 0.0
      );	                        
};
 
#endif
