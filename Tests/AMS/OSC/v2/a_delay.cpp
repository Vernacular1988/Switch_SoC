//a_delay.cpp
#include "a_delay.h"

// need an explicity delay element
//
  void a_delay::set_attributes() {
    outp.set_delay(delay);
  }

// note this cannot be a outp.write(initial)

  void a_delay::initialize()  {
    for(int i=0; i<delay; i++) {
      outp.initialize(initial,i);
    }
    cout << " initial "<< initial << "\n";
  }

  void a_delay::processing() {
    outp.write(inp.read());
  }

  // SCA_CTOR(a_delay) {} // below gives way to set values from parent 

  a_delay::a_delay(sc_core::sc_module_name n, double _init, int _delay) {
    initial=_init;
    delay=_delay;
  }

