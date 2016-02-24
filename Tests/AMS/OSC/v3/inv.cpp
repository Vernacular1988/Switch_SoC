// inv.cpp
// this one has the delay built in levitan@pitt.edu 10/15/2011
//

#include "inv.h"

// done first
void inv::set_attributes() {
  outp.set_delay(delay);
}

// done second  - before first time step
void inv::initialize() {
  for(int i=0; i<delay; i++) {
    outp.initialize(initial,i);
  }
  cout << " initial "<< initial << "\n";
}

// done every time
void inv::processing()
{
  double value;
  value = (VDD/2.0) * erfc(kslope*inp.read() - kslope);
  cout << value << "\n";
  outp.write(value);
}

// creation
inv::inv(sc_core::sc_module_name n, int d, double init, double k)
{
  delay = d;
  initial = init;
  kslope = k;
}
