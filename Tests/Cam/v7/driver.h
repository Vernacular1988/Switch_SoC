// driver.h
#ifndef DRIVERINC
#define DRIVERINC

#include "constants.h"

// structure of Associative Memory commands
typedef struct {
  command_type op;
  am_word msk;
  am_word cmp;
  am_word wd;
  sc_uint<AMSZ> ad;
} trans;

SC_MODULE(driver) {
  // IO ports
  sc_out<command_type> command;	    // Command to the AM
  sc_out<am_word>  maskout;	    // Comparison mask '1' = don't care
  sc_out<am_word>  compout;	    // Comparand
  sc_out<am_word> writedata;	    // for writing new data
  sc_out<sc_uint<AMSZ> > address;   // address for ram read/write operations
  sc_out<bool> start;		    // Start command out
  sc_in<bool> done;		    // Ready from AM
  sc_in<bool> match;		    // some/none match of comparand
  sc_in<am_word> readdata;	    // read back data from AM
  sc_in_clk clock; 		    // for synchronous operation

  // Internal Structure
  int count;			    // local count of commands
  trans com;			    // current transaction command
  void rundriver();		    // main process
  int set_next_command();	    // parse one line of program file

  // Constructor
  SC_CTOR(driver) {
    SC_THREAD(rundriver); 	    // Thread process (loops forever)
    sensitive << clock.pos();
    dont_initialize();              // wait for first clock
    count = 0;			    // initalize instruction count
  }
};

#endif
