// main.cpp

#include "pll.h"

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_NS);     //

  sca_tdf::sca_signal<double> source;
  sca_tdf::sca_signal<double> output1;

  sin_src sinin("sinin");
  sinin.outp(source);

  a_delay vco_delay("vco_delay");
  vco_delay.inp(source);
  vco_delay.outp(output1);

  drain drn1("drn1");
  drn1.in(output1);


/********* tracing signals */
  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "pll" );

  sca_util::sca_trace( atf, source, "source" );
  sca_util::sca_trace( atf, output1 ,"output1" );

  sc_start(20.0, SC_MS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}
