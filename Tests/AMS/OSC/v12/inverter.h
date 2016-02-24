// inverter.h
#ifndef INVERTER_H 
#define INVERTER_H

#include <systemc-ams.h>
#include "time.h"

// used to replace if/then/else code on max/min calculations
#define MIN(a,b) ((a)>(b))?(b):(a)
#define MAX(a,b) ((a)>(b))?(a):(b)

// should we use defines or const ? 

#define F 1.1f	    // step size adjustment factor ( >1 <2 ?)
#define VTOL1 0.1   // max voltage change tolerance 
#define VTOL2 1e-6  // min voltage change tolerance 
#define DTMIN 5e-15 // min time step 5 femto secs (larger than time_resolution)
#define CHANGE_THRESHOLD 0.05 //maximum change of inputs to invoke recalc.

// These should be dynamic since these caps are not fixed but are 
// really functions of the voltages across them and there are functions
// declared below that should calculate them if we want to be more accurate
//
#define Rf 100      // small series gate resistance
#define CGS 1e-15   // sum of gate to source caps
#define CGD 1e-15   // sum of gate to drain caps

const float VDD = 1.8f;
const float FACTOR=1.1f;
const float K=1.0f;

SC_MODULE(inverter) {
  // parameters to be assigned at instantiation
  char *NAME;
  float WLN, WLP, st0, DTMAX;
  // IO ports
  sc_in<float> i1_in, v2_in, dt1, dt2;
  sc_out<float> i2_out, v1_out, dt;

  int flag;	// shared flag between activations and calculus
  //  float dt; this is an output port (or shared flag?)
 
  float Ipmos (float vi, float vout);
  float Inmos (float vi, float vout);
  float Cgs (float wl, float cox, float vgs, float vds, float vth);
  float Cgd (float wl, float cox, float vgs, float vds, float vth);
  float Cgb (float wl, float cox, float vgs, float vds, float vth);

  sc_event activation_event;
  void calculus();
  void activation1();
  void activation2();

  SC_CTOR(inverter) {
    SC_THREAD(activation1);
    sensitive << i1_in;
    SC_THREAD(activation2);
    sensitive << v2_in;
    SC_THREAD(calculus);
    sensitive << activation_event;
  };
};


#endif
