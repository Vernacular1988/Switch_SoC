// main.cpp

#include "drain.h"
#include "top.h"

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_NS);     // for solvers

  sca_tdf::sca_signal<double> vco1;
  sca_tdf::sca_signal<double> vco2;
  sca_tdf::sca_signal<double> vco3;
  sca_tdf::sca_signal<double> lpf1;
  sca_tdf::sca_signal<double> lpf2;
  sca_tdf::sca_signal<double> lpf3;


/********** instantiating library-modules**********/
  top top1("top1");         // using all defaults in top.h
  top1.vco3_out(vco3);
  top1.vco2_out(vco2);
  top1.vco1_out(vco1);
  top1.lpf_out1(lpf1);
  top1.lpf_out2(lpf2);
  top1.lpf_out3(lpf3);

  // sinks for unused output signals
  drain drn1("drn1");
  drn1.in(lpf1);
  drn1.set_timestep(10.0, SC_US);

  drain drn2("drn2");
  drn2.in(lpf2);

  drain drn3("drn3");
  drn3.in(lpf3);

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