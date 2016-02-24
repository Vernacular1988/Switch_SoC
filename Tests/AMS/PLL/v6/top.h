//top.h

#ifndef TOP_H
#define TOP_H

#include <systemc-ams.h>
#include "tuv_ams_library/tuv_ams_library.h"
#include "osc.h"
#include "split.h"

using namespace TUV_ams_lib::bb;

SC_MODULE(top) {

  /********** defining ios *********** */

  sca_tdf::sca_out<double> vco1_out;
  sca_tdf::sca_out<double> vco2_out;
  sca_tdf::sca_out<double> vco3_out;
  sca_tdf::sca_out<double> lpf_out1;
  sca_tdf::sca_out<double> lpf_out2;
  sca_tdf::sca_out<double> lpf_out3;



  /********** defining signals *********** */
 private:
  sca_tdf::sca_signal<double> sw12, sw13, 
    sw21, sw23, 
    sw31, sw32, 
    sint1, sint2;
  sca_tdf::sca_signal<double> 
    vco1_in1, vco1_in2, 
    vco2_in1, vco2_in2, 
    vco3_in1, vco3_in2;

 /********** defining parameters in defaults below  *********** */

  //************* sub blocks *********************/

  osc *osc1, *osc2, *osc3;
  split *sp1, *sp2, *sp3;

 public: // constructor 
  top(sc_core::sc_module_name n, 
      double phc_gain = 100.0,	// phase comparitor gain
      double lp_fc = 10.0,	// low pass filter cut off freq
      double vco_freq = 1000.0, // natural frequency of vco
      double kvco = 100.0,	// control sensitivity for vco
      double vco_gain = 1.0	// amplitude gain of vco
      );	                        
};

#endif
