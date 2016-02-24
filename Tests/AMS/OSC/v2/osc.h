// osc.h
#ifndef OSC_H
#define OSC_H

#include <systemc-ams.h>
//#include "tuv_ams_library/tuv_ams_library.h"
//using namespace TUV_ams_lib::bb;

#include "inv.h"
#include "a_delay.h"

#include <math.h>
#define VDD 1.8

SC_MODULE(osc) {

 public:
  sca_tdf::sca_out<double> inp;        // input port
  sca_tdf::sca_out<double> outp;	// output port
  sca_eln::sca_terminal inoutp;

 private:
  sca_tdf::sca_signal<double> inv_out, inv_in, del_in, del_out;
  sca_eln::sca_node v0, v1;
  sca_eln::sca_node_ref gnd;
  inv *inv1;		// invirters with weighted sum input and pll generator 
  a_delay *del1;     // delay in feedback loops

  // electrial network components
  sca_eln::sca_tdf::sca_vsink *vsnk;
  sca_eln::sca_tdf::sca_vsource *vsrc;
  sca_eln::sca_r *rc, *rs;
  sca_eln::sca_c *c1;
  sca_eln::sca_l *l1;

 public:

  osc(sc_core::sc_module_name n, double ic = VDD/4.0, int delay=25, 
      double R=1e4, double C=1e-15);

  //  dont_initialize();
};

#endif
