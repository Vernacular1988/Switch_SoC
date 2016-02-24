// a_vco.h
#ifndef VCO_H
#define VCO_H

#include <systemc-ams.h>
//#include "tuv_ams_library/tuv_ams_library.h"
//using namespace TUV_ams_lib::bb;


SCA_TDF_MODULE(my_vco) {

 public:
  sca_tdf::sca_in<double> in;        // input port
  sca_tdf::sca_out<double> out;	// output port

 private:

  int rate;		// The datarate of the output
  double freq;          // Central frequency [Hz]
  double kvco;          // Sensitivity [Hz/V]
  double gain;          // Gain

  double sample_time;
  double stepsize;
  double actval;


  void set_attributes() ;

  void initialize() ;

  void processing();

 public:

  my_vco(sc_core::sc_module_name n, double freq_data, int datarate, double _kvco, double _gain, double _phase = 0.0);
};

#endif
