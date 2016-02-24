//file inv.h
//
// inverter modeled as erfc function and delay element
// levitan@pitt.edu 10/12/2011

#ifndef INV_H
#define INV_H

#include <systemc-ams.h>

#include <math.h>
#define VDD 1.8

SCA_TDF_MODULE(inv) {

 public:
  sca_tdf::sca_in<double> inp;        // input port
  sca_tdf::sca_out<double> outp;	// output port

 private:
  double kslope;      // slope control for transfer curve (2.0-4.0 seems good)
  double initial;     // inital output value, needed for delayed blocks
  double delay;       // intrinsic delay for inverter

  void set_attributes() ;

  void initialize() ;

  void processing();

 public:

  inv(sc_core::sc_module_name n, int d=10, double init=0.0, double k=3.0);

};

#endif
