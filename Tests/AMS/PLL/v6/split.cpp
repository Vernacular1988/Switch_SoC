//split.cpp
#include "split.h"

void split::initialize() {
  out1.write(0.0);
  out2.write(0.0);
}

void split::processing() {
    temp = in.read();
    out1.write(temp);
    out2.write(temp);
}

split::split(sc_core::sc_module_name n) {}
