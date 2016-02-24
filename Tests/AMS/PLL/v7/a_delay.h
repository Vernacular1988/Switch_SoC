// a_delay.h

#ifndef A_DELAY_H
#define A_DELAY_H

#include <systemc-ams.h>

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
  //  a_delay::a_delay(sc_core::sc_module_name n, double _init=0.0, int _delay=1);
  a_delay(sc_core::sc_module_name n, double _init=0.0, int _delay=1);
};

#endif
