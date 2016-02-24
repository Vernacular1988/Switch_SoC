// monitor.h
#ifndef MONITOR_H
#define MONITOR_H

#include <systemc-ams.h>
SCA_TDF_MODULE(monitor) {

  sca_tdf::sca_in<double> inp;

 private:
  void processing(double *);

};

#endif
