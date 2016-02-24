// main.cpp

#include <time.h>
#include <math.h>
#include <systemc-ams.h>
#include "osc.h"

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_FS);     // for solvers

  sca_tdf::sca_signal<double> o11,o12,o21,o22;
  sca_eln::sca_node v0, v1, v2;
  sca_eln::sca_node_ref gnd;

  osc *osc1, *osc2;

  // electrial network components
  //sca_eln::sca_tdf::sca_vsink *vsnk;
  //sca_eln::sca_tdf::sca_vsource *vsrc;
  sca_eln::sca_r *r1, *r2, *r3;
  //sca_eln::sca_c *c1;
  //sca_eln::sca_l *l1;

  //srand(time(NULL));

  /********** instantiating library-modules**********/

  // generate inverters and connections
  osc1 = new osc("osc1", 1.8, 15, 1e4, 1e-15);
  osc1->inoutp(v0);
  osc1->inp(o11);
  osc1->outp(o12);
  
  osc2 = new osc("osc2", 0.0, 15, 1e4, 1e-15);
  osc2->inoutp(v1);
  osc2->inp(o21);
  osc2->outp(o22);

  r1 = new sca_eln::sca_r("R1",1e5);
  r1->p(v0);
  r1->n(v2);

  r2 = new sca_eln::sca_r("R2",1e5);
  r2->p(v1);
  r2->n(v2);

  r3 = new sca_eln::sca_r("R3",1e12);
  r3->p(v2);
  r3->n(gnd);


    /********* tracing signals *************************/

  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "inv" );

  sca_util::sca_trace(atf,v0, "v0");
  sca_util::sca_trace(atf,v1, "v1");
  sca_util::sca_trace(atf,v2, "v2");

  sca_util::sca_trace(atf,o11, "o11");
  sca_util::sca_trace(atf,o12, "o12");
  sca_util::sca_trace(atf,o21, "o21");
  sca_util::sca_trace(atf,o22, "o22");

  sc_start(10.0, SC_NS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}
