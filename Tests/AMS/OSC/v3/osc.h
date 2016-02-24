// osc.h
#ifndef OSC_H
#define OSC_H

#include <systemc-ams.h>


#include "inv.h"

#include <math.h>
#define VDD 1.8
//
// oscilator is inverter(& internal delay)  plus linear network for feedback
//

SC_MODULE(osc) {

 public:
  sca_tdf::sca_out<double> out1, out2;	// output ports for debugging
  sca_eln::sca_terminal inoutp;         // real electrical port

 private:

  // tdf signals (not used since we use ports out1, out2) 
  sca_tdf::sca_signal<double> inv_out, inv_in;

  // inverter tdf model
  inv *inv1;		// our inverter model with delay 

  // eln signals 
  sca_eln::sca_node v0, v1;
  sca_eln::sca_node_ref gnd;

  // electrial network components
  sca_eln::sca_tdf::sca_vsink *vsnk;  // eln to tdf converter
  sca_eln::sca_tdf::sca_vsource *vsrc; // tdf to eln converter
  sca_eln::sca_r *r1, *rc;            // r l c for linear feedback
  sca_eln::sca_c *c1;
  sca_eln::sca_l *l1;

 public:

  osc(sc_core::sc_module_name n, double ic = 0.0, int delay=10, 
      double R=1e4, double C=1e-15);
};

#endif
