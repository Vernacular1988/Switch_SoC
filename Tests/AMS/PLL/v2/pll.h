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

  void drain::processing() {}

  SCA_CTOR(drain) {}
};


// analog delay for pll based on digitial delay in TUV BB

SCA_TDF_MODULE(a_delay) {

  sca_tdf::sca_in<double> inp;
  sca_tdf::sca_out<double> outp;

  int delay;
  double initial;

  void a_delay::set_attributes() {
    outp.set_delay(delay);
  }

  // note this cannot be a outp.write(initial)
  void a_delay::initialize()  {
    outp.initialize(initial);
  }

  void a_delay::processing() {
    outp.write(inp.read());
  }

  // SCA_CTOR(a_delay) {} // below gives way to set values from parent 

  a_delay::a_delay(sc_core::sc_module_name n, double _init=0.0, int _delay=1) {
    initial=_init;
    delay=_delay;
  }

};

#endif
