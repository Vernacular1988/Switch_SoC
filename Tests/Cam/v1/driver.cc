// driver.cc

#include "driver.h"

void driver::rundriver() {
  compout = 0;
  maskout = 0;
  while (true) {
#if 1
// this drives based on done and the timing of the master clock
       if(done) {
	    
           start = true;
           wait();
           start = false;
           wait();
         compout = ++count;
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
