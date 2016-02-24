// main.cpp

#include "drain.h"
#include "top.h"

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_NS);     // for solvers

  sca_tdf::sca_signal<double> vco_out_3;
  sca_tdf::sca_signal<double> vco_out_2;
  sca_tdf::sca_signal<double> vco_out_1;
  sca_tdf::sca_signal<double> lpf_out1;
  sca_tdf::sca_signal<double> lpf_out2;
  sca_tdf::sca_signal<double> lpf_out3;

/********** instantiating library-modules**********/
  top top1("top1");
  top1.vco_out_3(vco_out_3);
  top1.vco_out_2(vco_out_2);
  top1.vco_out_1(vco_out_1);
  top1.lpf_out1(lpf_out1);
  top1.lpf_out2(lpf_out2);
  top1.lpf_out3(lpf_out3);

  // sinks for unused output signals
  drain drn1("drn1");
  drn1.in(lpf_out1);
  drn1.set_timestep(10.0, SC_US);

  drain drn2("drn2");
  drn2.in(lpf_out2);

  drain drn3("drn3");
  drn3.in(lpf_out3);

/********* tracing signals *************************/
  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "pll" );

  sca_util::sca_trace( atf, vco_out_1, "vco_out_1" );
  sca_util::sca_trace( atf, vco_out_2 ,"vco_out_2" );
  sca_util::sca_trace( atf, vco_out_3 ,"vco_out_3" );
  sca_util::sca_trace( atf, lpf_out1 ,"lpf_out1" );
  sca_util::sca_trace( atf, lpf_out2 ,"lpf_out2" );
  sca_util::sca_trace( atf, lpf_out3 ,"lpf_out3" );

  sc_start(100.0, SC_MS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}
