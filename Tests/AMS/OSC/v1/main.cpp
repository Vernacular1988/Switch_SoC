// main.cpp

#include <time.h>
#include <math.h>
#include <systemc-ams.h>
#include "osc.h"
#include "a_delay.h"

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_FS);     // for solvers

  sca_tdf::sca_signal<double> osc_out, osc_in, del_out;
  sca_eln::sca_node v0, v1, v2;
  sca_eln::sca_node_ref gnd;

  char  mn[64];			// used to create unique module name strings

  osc *osc1;		// oscilators with weighted sum input and pll generator 
  a_delay *del1;     // delay for 90 degree phase delay in feedback loops
  int temp;

  // electrial network components
  sca_eln::sca_tdf::sca_vsink *vsnk;
  sca_eln::sca_tdf::sca_vsource *vsrc;
  sca_eln::sca_r *r1, *rs;
  sca_eln::sca_c *c1;
  sca_eln::sca_l *l1;

  srand(time(NULL));

  /********** instantiating library-modules**********/

  // generate oscilators and connections
  sprintf(mn,"%s%i","osc",1);
  osc1 = new osc(mn);
  osc1->outp(osc_out);
  osc1->inp(osc_in);
  osc1->set_timestep(1, SC_PS); 
  
  sprintf(mn,"%s%i","del",1);
  del1 = new a_delay(mn, 0.7, 25); // in timestep units
  del1->inp(osc_out);
  del1->outp(del_out);

  vsrc = new sca_eln::sca_tdf::sca_vsource("vsrc");
  vsrc->inp(del_out);
  vsrc->p(v0);
  vsrc->n(gnd);

  rs = new sca_eln::sca_r("RS", 1.0e4);
  rs->p(v0);
  rs->n(v1);

  c1 = new sca_eln::sca_c("C1", 1.0e-15);
  c1->p(v1);
  c1->n(gnd);

  // r1 = new sca_eln::sca_r("R1", 1.0e6);
  // r1->p(v1);
  // r1->n(v2);

  // l1 = new sca_eln::sca_l("L1", 1.0e-9);
  // l1->p(v1);
  // l1->n(v2);

  vsnk = new sca_eln::sca_tdf::sca_vsink("vsnk");
  vsnk->outp(osc_in);
  vsnk->p(v1);
  vsnk->n(gnd);


    /********* tracing signals *************************/

  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "osc" );

  sprintf(mn,"%s%i","osc_out",1);
  sca_util::sca_trace( atf, osc_out, mn);
  sca_util::sca_trace(atf,del_out, "del_out");
  sca_util::sca_trace(atf,v0, "v0");
  sca_util::sca_trace(atf,v1, "v1");
  sca_util::sca_trace(atf,v2, "v2");


  sprintf(mn,"%s%i","osc_in",1);
  sca_util::sca_trace( atf, osc_in, mn);

  sc_start(10.0, SC_NS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}
