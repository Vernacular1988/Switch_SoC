// pll.h

#ifndef PLL_H
#define PLL_H

#include <systemc-ams.h>

SCA_TDF_MODULE(drain)
{
  sca_tdf::sca_in<double> in;

  SCA_CTOR(drain) {}
};


// analog delay

SCA_TDF_MODULE(a_delay) {

  sca_tdf::sca_in<double> inp;
  sca_tdf::sca_out<double> outp;

   void set_attributes() { 
     outp.set_delay(1); 
   } 

  void initialize()  {
    outp.initialize(1.0);
  }

  void processing() {
    outp.write(inp.read());
  }

  SCA_CTOR(a_delay) {}

};

SCA_TDF_MODULE(sin_src)
{
  sca_tdf::sca_out<double> outp;
  double ampl;
  double freq;

  void set_attributes () {
    set_timestep(1.0, SC_US);
    outp.set_rate(1);
  }

  void processing(){
    outp.write(sin(2*M_PI * 100000 * sc_time_stamp().to_seconds()));
  }

  SCA_CTOR(sin_src) {}
};


#endif
