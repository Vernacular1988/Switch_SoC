// osc.cpp

#include "osc.h"

void osc::initialize()
{

}

void osc::set_attributes()
{
  //  set_timestep(1.0, sc_core::SC_NS);
}

void osc::processing()
{
  double value;
  value = (VDD/2.0) * erfc(3.0*inp.read() - 3.0);
  cout << value << "\n";
  outp.write(value);

  //outp =  VDD - inp.read();
}

osc::osc(sc_core::sc_module_name n)
{

}
