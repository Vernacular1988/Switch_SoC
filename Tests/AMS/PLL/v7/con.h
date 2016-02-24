// con.h
#ifndef CON_H
#define CON_H

#include <systemc-ams.h>
SCA_TDF_MODULE(constant) {

  sca_tdf::sca_out<double> out;

 private:
  double initial;
  void initialize();
  void processing();

 public:
  constant(sc_core::sc_module_name n, double init=0.0);
};

#endif
