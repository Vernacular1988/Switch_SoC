// driver.cc

#include "driver.h"

#define DEBUG 1

#define NUMCOM 9
static trans command_list[NUMCOM] =
//	OP	mask	comp	data	addr
    {WRITE,	0,	0,	0x13,	3,	// write  0x13 to 3
     WRITE,	0,	0,	0x27,	4,	// write  0x27 to 4
     READ,	0,	0,	0,	3,	// read from 3
     READ,	0,	0,	0,	4,	// read from 4
     READ,	0,	0,	0,	5,	// read from 5
     COMPARE,	0,	0x13,   0,	0,	// compare for 0x13
     COMPARE,	0,	0x17,   0,	0,	// compare for 0x17
     COMPARE,	0xF,	0x20,   0,	0, 	// compare for 0x2*
     AREAD,	0,	0,	0,	0	// associative read
    };

void driver::set_next_command(int cnt) {
  com = command_list[cnt]; 
}


void driver::rundriver() {
  compout = 0;
  maskout = 0;

  // Run continuously/synchronously while there are commands to do
  while (count < NUMCOM) {
    if(DEBUG) cout << " Driver command number: "<< count << endl;

    if(done) {
      set_next_command(count++);
      if(DEBUG) cout << "Driver command is: "<< com.op << endl;
      command = com.op;
      maskout = com.msk;
      compout = com.cmp;
      writedata = com.wd;
      address = com.ad;
	 
      start = true;
      wait();
      start = false;
      wait();
    }
    wait(); // synchronosly wait for for AM ready (done)
  }
}
