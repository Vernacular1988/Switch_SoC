// main.cpp

#include "drain.h"
#include "top.h"

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_NS);     // for solvers

  sca_tdf::sca_signal<double> modulation;
  sca_tdf::sca_signal<double> source;
  sca_tdf::sca_signal<double> vco_out_delay;
  sca_tdf::sca_signal<double> lpf_out;

/********** instantiating library-modules**********/
  top top1("top1");
  top1.source(source);
  top1.modulation(modulation);
  top1.vco_out_delay(vco_out_delay);
  top1.lpf_out(lpf_out);

  // sink for unused output signal
  drain drn2("drn2");
  drn2.in(lpf_out);


/********* tracing signals *************************/
  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "pll" );

  sca_util::sca_trace( atf, source, "source" );
  sca_util::sca_trace( atf, modulation ,"modulation" );
  sca_util::sca_trace( atf, vco_out_delay ,"vco_out_delay" );
  sca_util::sca_trace( atf, lpf_out ,"lpf_out" );

  sc_start(1000.0, SC_MS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}
