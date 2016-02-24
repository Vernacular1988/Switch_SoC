// sin_crc.h
#ifndef SIN_SRC_H_
#define SIN_SRC_H_

#include<systemc-ams.h>

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
