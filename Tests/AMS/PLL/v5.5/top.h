//top.h

#ifndef TOP_H
#define TOP_H

#include <systemc-ams.h>
#include "tuv_ams_library/tuv_ams_library.h"
#include "drain.h"
#include "a_delay.h"
#include "pll.h"

using namespace TUV_ams_lib::bb;

SC_MODULE(top) {

  /********** defining ios *********** */

  sca_tdf::sca_out<double> vco_out_1;
  sca_tdf::sca_out<double> vco_out_2;
  sca_tdf::sca_out<double> vco_out_3;
  sca_tdf::sca_out<double> lpf_out1;
  sca_tdf::sca_out<double> lpf_out2;
  sca_tdf::sca_out<double> lpf_out3;

  /********** defining signals *********** */
 private:
  sca_tdf::sca_signal<double> pco;
  sca_tdf::sca_signal<double> vco_out;

 /********** defining parameters in defaults below  *********** */
  // sine wave generator control not used
  //bool		s_fc;	// sine freq control port active
  //bool		s_ac;	// sine amp control port active

  // analog vco control

  //************* sub blocks *********************/
  sine* sin1;
  my_pll *pll1, *pll2, *pll3;

 public: // constructor 
  top(sc_core::sc_module_name n, 
      double s_freq = 1000,	// sine base frequency	 
      double m_freq = 10.0,	// sine modulator frequency
      double s_amp = 1.0,	// sine amplitude 
      double s_ph = 0.0,	// sine phase
      double s_offset = 0.0,	// sine offset
      int s_rate = 1,		// sine sampling rate
      double phc_gain = 100.0,	// phase comparitor gain
      double lp_fc = 10.0,	// low pass filter cut off freq
      double vco_freq = 1000.0, // natural frequency of vco
      double kvco = 100.0,	// control sensitivity for vco
      double vco_gain = 1.0	// amplitude gain of vco
      );	                        

};
 
#endif
