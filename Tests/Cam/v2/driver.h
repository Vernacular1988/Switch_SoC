// driver.h
#ifndef DRIVERINC
#define DRIVERINC

#include "systemc.h"
#include "cam.h"

#define NUMCOM 2

typedef struct {
  command_type op;
  am_word msk;
  am_word cmp;
  am_word wd;
  sc_uint<AMSZ> ad;
} trans;



SC_MODULE(driver) {
  sc_out<command_type> command;
  sc_out<am_word>  maskout;
  sc_out<am_word>  compout;
  sc_out<am_word> writedata; // for writing new data
  sc_out<sc_uint<AMSZ> > address; // address for ram read/write operations
  sc_out<bool> start; 
  sc_in<bool> done;
  sc_in<bool> match;
  sc_in<am_word> readdata;
  sc_in_clk clock; // input port
  int count;		// local count of commands
  trans com;		// current transaction command
  void rundriver();
  void set_next_command(int);

  // Constructor
  SC_CTOR(driver) {
    SC_THREAD(rundriver); // Thread process
    sensitive << clock.pos();
    dont_initialize(); // wait for first clock
    count = 0;
    //compout = 0;
    //maskout = 0;
  }
};

#endif
