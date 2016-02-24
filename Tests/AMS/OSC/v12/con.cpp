//con.cpp
#include "con.h"

  // note this cannot be a outp.write(initial)
  void constant::initialize()  {
    out.initialize(initial);
  }

  void constant::processing() {
    out.write(initial);
  }

  constant::constant(sc_core::sc_module_name n, double init) {
    initial=init;
  }

