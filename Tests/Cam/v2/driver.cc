// driver.cc

#include "driver.h"

trans command_list[NUMCOM] = {WRITE, 0, 0, 13, 3,
			 WRITE, 0, 0, 27, 4};



void driver::set_next_command(int cnt) {
  extern trans  command_list[NUMCOM];

  com = command_list[cnt]; 
}


void driver::rundriver() {
  compout = 0;
  maskout = 0;

  while (count < NUMCOM) {
    cout << "Driver command number: "<< count << endl;

#if 1
    // this drives based on done and the timing of the master clock
    if(done) {
      set_next_command(count++);

      cout << "Driver command is: "<< com.op << endl;
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
#else
    // this drives based only on a handshake with done 
    if(done) {
      start = true;
    } else {
      start = false;
      compout = ++count;
    }
#endif

    wait();
  }
}
