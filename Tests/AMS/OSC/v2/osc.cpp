// osc.cpp

#include "osc.h"

osc::osc(sc_core::sc_module_name n, double ic, int delay, double R, double C)
{

  cout << "OSC "<< n << " ic "<< ic << " delay " << delay 
       << " R " << R << " C " << C << "\n";

  inv1 = new inv("inv",2.0); // slope factor of 2.0 - 4.0 seems good
  inv1->outp(outp);
  inv1->inp(inp);
  inv1->set_timestep(1, SC_PS); 

  
  vsrc = new sca_eln::sca_tdf::sca_vsource("vsrc");
  vsrc->inp(outp);
  vsrc->p(v0);
  vsrc->n(gnd);

  rs = new sca_eln::sca_r("RS", R);
  rs->p(v0);
  rs->n(v1);

  c1 = new sca_eln::sca_c("C1", C);
  c1->p(v1);
  c1->n(gnd);

  vsnk = new sca_eln::sca_tdf::sca_vsink("vsnk");
  vsnk->outp(del_in);
  vsnk->p(v1);
  vsnk->n(gnd);

  del1 = new a_delay("del1", ic, delay); // in timestep units
  del1->inp(del_in);
  del1->outp(inp);

  rc = new sca_eln::sca_r("RC", 1e1);
  rc->p(v1);
  rc->n(inoutp);

  //  dont_initialize();
}
