// driver.h
#ifndef DRIVERINC
#define DRIVERINC

#include "systemc.h"
#include "cam.h"

SC_MODULE(driver) {
  sc_out<am_word>  maskout;
  sc_out<am_word>  compout;
  sc_in_clk clock; // input port
  sc_out<bool> start; 
  sc_in<bool> done; 

  int count;
  void rundriver();

  // Constructor
  SC_CTOR(driver) {
    SC_THREAD(rundriver); // Thread process
    sensitive << clock.pos();
    count = 0;
    //compout = 0;
    //maskout = 0;
  }
};

#endif
