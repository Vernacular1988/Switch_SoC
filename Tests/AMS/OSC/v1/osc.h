// osc.h
#ifndef OSC_H
#define OSC_H

#include <systemc-ams.h>
//#include "tuv_ams_library/tuv_ams_library.h"
//using namespace TUV_ams_lib::bb;

#include <math.h>
#define VDD 1.8

SCA_TDF_MODULE(osc) {

 public:
  sca_tdf::sca_in<double> inp;        // input port
  sca_tdf::sca_out<double> outp;	// output port

 private:

  void set_attributes() ;

  void initialize() ;

  void processing();

 public:

  osc(sc_core::sc_module_name n);

};

#endif
