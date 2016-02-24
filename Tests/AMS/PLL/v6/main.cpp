// main.cpp

//#include "top.h"

#include <systemc-ams.h>
//#include "tuv_ams_library/tuv_ams_library.h"
#include "osc.h"
#include "con.h"

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_NS);     // for solvers

  sca_tdf::sca_signal<double> vco1;
  sca_tdf::sca_signal<double> vco2;
  sca_tdf::sca_signal<double> vco3;
  sca_tdf::sca_signal<double> lpf1;
  sca_tdf::sca_signal<double> lpf2;
  sca_tdf::sca_signal<double> lpf3;

 sca_tdf::sca_signal<double> 
   sw12, sw13, 
   sw21, sw23, 
   sw31, sw32;

 double phc_gain = 100.0;	// phase comparitor gain
 double lp_fc = 10.0;		// low pass filter cut off freq
 double vco_freq = 1000.0; 	// natural frequency of vco
 double kvco = 100.0;		// control sensitivity for vco
 double vco_gain = 1.0;		// amplitude gain of vco

/********** instantiating library-modules**********/


// generate constant signals for weights
 constant con0("con0", 1.0);
 con0.out(sw12);
 con0.set_timestep(10.0, SC_US);

 constant con1("con1", 1.0);
 con1.out(sw13);

 constant con2("con2", 1.0);
 con2.out(sw21);

 constant con3("con3", 1.0);
 con3.out(sw23);

 constant con4("con4", 1.0);
 con4.out(sw31);

 constant con5("con5", 1.0);
 con5.out(sw32);


  osc osc1("osc1", phc_gain, lp_fc, vco_freq, kvco, vco_gain);
  osc1.vco_ref1(vco2);
  osc1.vco_ref2(vco3);
  osc1.vco_out(vco1);
  osc1.lpf_out(lpf1);
  osc1.w1(sw21);
  osc1.w2(sw31);

  osc osc2("osc2", phc_gain, lp_fc, vco_freq+10, kvco, vco_gain);
  osc2.vco_ref1(vco1);
  osc2.vco_ref2(vco3);
  osc2.vco_out(vco2);
  osc2.lpf_out(lpf2);
  osc2.w1(sw12);
  osc2.w2(sw32);

  osc osc3("osc3", phc_gain, lp_fc, vco_freq+20, kvco, vco_gain);
  osc3.vco_ref1(vco1);
  osc3.vco_ref2(vco2);
  osc3.vco_out(vco3);
  osc3.lpf_out(lpf3);
  osc3.w1(sw13);
  osc3.w2(sw23);


/********* tracing signals *************************/
  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "pll" );

  sca_util::sca_trace( atf, vco1, "vco1" );
  sca_util::sca_trace( atf, vco2 ,"vco2" );
  sca_util::sca_trace( atf, vco3 ,"vco3" );
  sca_util::sca_trace( atf, lpf1 ,"lpf1" );
  sca_util::sca_trace( atf, lpf2 ,"lpf2" );
  sca_util::sca_trace( atf, lpf3 ,"lpf3" );

  sc_start(100.0, SC_MS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}
