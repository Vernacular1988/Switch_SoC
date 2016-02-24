// driver.cc

#include "driver.h"

// 0 1 or 2
#define DEBUG 2

#define NUMCOM 20
static trans command_list[NUMCOM] = {
  //     OP	mask	comp	data	addr
  SETTAG,	0,	0,	0,	0,	// set all tags
  CLEARTAG,	0,	0,	0,	0,	// clear all tags
  COMPTAG,	0,	0,	0,	0,	// complement all tags
  WRITE,	0,	0,	0x13,	3,	// write 0x13 to 3
  WRITE,	0,	0,	0x27,	4,	// write 0x27 to 4
  WRITE,	0,	0,	0X13,	13,	// write 0x13 to 13
  READ,		0,	0,	0,	3,	// read from 3
  READ,		0,	0,	0,	4,	// read from 4
  READ,		0,	0,	0,	5,	// read from 5
  COMPARE,	0,	0x13, 	0,	0,	// compare for 0x13
  HAMMING,	0xF0,	0x03,	0,	0,	// haming distance for 000000*3
  KNN,		0,	0,	0,	0,	// K nearest Neighbors
  WTA,		0,	0,	0,	0,	// winner take all
  AREAD,	0,	0,	0,	0,	// read the winner
  SELFIRST,	0,	0,	0,	0,	// turn off second match
  COMPARE,	0,	0x17, 	0,	0,	// compare for 0x17
  COMPARE,	0xF,	0x20, 	0,	0, 	// compare for 0x2* (finds 27)
  AREAD,	0,	0,	0,	0,	// associative read 
  WRITE,	0x13,	0,	0x08,	3,	//masked write to 3 
  READ,		0,	0,	0,	3	// read from 3 (1B, 8+3=11)
};

void driver::set_next_command(int cnt) {
  com = command_list[cnt]; 
}


void driver::rundriver() {
  compout = 0;
  maskout = 0;

  // Run continuously/synchronously while there are commands to do
  while (count < NUMCOM) {
    if(DEBUG) cout << "Driver command number: "<< count << endl;

    if(done) {
      set_next_command(count++);
      if(DEBUG)   cout << "Driver command is: "<< com.op << endl;
      if(DEBUG>1) cout << "Mask:\t\t" << com.msk << endl
		       << "Comparand:\t" << com.cmp << endl
		       << "Write Data:\t" << com.wd  << endl
		       << "Address:\t" << com.ad << endl;
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
