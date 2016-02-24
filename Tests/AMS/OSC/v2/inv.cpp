// inv.cpp

#include "inv.h"

void inv::initialize()
{
}

void inv::set_attributes()
{
}

void inv::processing()
{
  double value;
  value = (VDD/2.0) * erfc(kslope*inp.read() - kslope);
  cout << value << "\n";
  outp.write(value);
}

inv::inv(sc_core::sc_module_name n, double k)
{
  kslope = k;
}
