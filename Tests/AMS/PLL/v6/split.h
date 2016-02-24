//split.h
#ifndef SPLIT_H
#define SPLIT_H

#include <systemc-ams.h>

// sink of data 
SCA_TDF_MODULE(split)
{
  sca_tdf::sca_in<double> in;
  sca_tdf::sca_out<double> out1;
  sca_tdf::sca_out<double> out2;

 private:
  double temp;
  void initialize();
  void processing();

 public:
  split(sc_core::sc_module_name n);

};


#endif
