//drain.h
#ifndef DRAIN_H
#define DRAIN_H

#include <systemc-ams.h>

// sink of data 
SCA_TDF_MODULE(drain)
{
  sca_tdf::sca_in<double> in;

  void drain::processing() {}

  SCA_CTOR(drain) {}
};


#endif
