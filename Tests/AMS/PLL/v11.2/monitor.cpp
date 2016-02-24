//monitor.cpp
// monitor an internal signal value
#include "monitor.h"

  void monitor::processing(double *sig) {
    *sig = inp.read();
  }


