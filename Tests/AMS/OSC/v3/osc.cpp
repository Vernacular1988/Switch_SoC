// osc.cpp

#include "osc.h"

osc::osc(sc_core::sc_module_name n, double ic, int delay, double R, double C)
{

  cout << "OSC "<< n << " ic "<< ic << " delay " << delay 
       << " R " << R << " C " << C << "\n";

  inv1 = new inv("inv", delay, ic, 2.0); // slope factor of 2.0 - 4.0 seems good
  inv1->inp(out1);
  inv1->outp(out2);
  inv1->set_timestep(1, SC_PS); 
  
  // tdf to eln voltage source converter
  vsrc = new sca_eln::sca_tdf::sca_vsource("vsrc");
  vsrc->inp(out2);
  vsrc->p(v0);
  vsrc->n(gnd);

  // rc low pass filter
  r1 = new sca_eln::sca_r("R1", R);
  r1->p(v0);
  r1->n(v1);

  c1 = new sca_eln::sca_c("C1", C);
  c1->p(v1);
  c1->n(gnd);

  // eln to tdf voltage converter
  vsnk = new sca_eln::sca_tdf::sca_vsink("vsnk");
  vsnk->outp(out1);
  vsnk->p(v1);
  vsnk->n(gnd);

  // resitive coupling to electrical (bi-directional conservative port)
  rc = new sca_eln::sca_r("RC", 1e6);
  rc->p(v1);
  rc->n(inoutp);

}
