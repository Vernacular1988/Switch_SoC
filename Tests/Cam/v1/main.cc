// main.cc
#include "cam.h"
#include "driver.h"

int sc_main(int argc, char* argv[]) {

  sc_clock CLOCK("clock", 20, SC_NS); // transmit clock
  sc_signal<am_word> comparand;
  sc_signal<am_word> mask;
  sc_signal<bool> mstart, mdone, mmatch;
 


  //cam_word fred[10];
  //  comparand = 16045690984833335000, //two dead beef 
  //  mask = "0010";

  amem am1("amem");
  am1.compin(comparand);
  am1.maskin(mask);
  am1.start(mstart);
  am1.done(mdone);
  am1.match(mmatch);
  am1.clock(CLOCK);

  driver dr1("driver");
  dr1.compout(comparand);
  dr1.maskout(mask);
  dr1.clock(CLOCK);
  dr1.start(mstart);
  dr1.done(mdone);




  //  fred[3].word = "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";
  //fred[3].word[1] = 1;

//   for(int i = 0 ; i<10; i++) {
//     fred[i].word = (i | comparand);
//     cout << fred[i];

//     //    if (fred[i].word == comparand) {
//     //  cout << "match \n";
//     //}

//     cout << comparand << " Comparand" << endl << mask <<" Mask" << endl 
// 	 << ((fred[i].word ^ comparand) & ~mask) << " Result" << endl;

//     if (((fred[i].word ^ comparand) & ~mask) == 0) 
//       { 
// 	fred[i].tag = 1; 
//       } else {
// 	fred[i].tag = 0;
//       }

//     cout << fred[i]  << endl;
//   }



  // tracing:
  // trace file creation
  sc_trace_file *tf = sc_create_vcd_trace_file ("cam");

  // External Signals
  sc_trace(tf, CLOCK.signal(), "clock");
  sc_trace(tf, comparand, "comp");
  sc_trace(tf, mask, "mask");
  sc_trace(tf, mstart, "start");
  sc_trace(tf, mdone, "done");
  sc_trace(tf, mmatch, "match");

  sc_start(1000, SC_NS);
  sc_close_vcd_trace_file(tf);
  return(0);
}

