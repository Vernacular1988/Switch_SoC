//cam.h
#ifndef CAMINC
#define CAMINC

#include "systemc.h"
#include "constants.h"
#include "knn.h"  // needed because it defines the sub-module

struct cam_word {
  am_word word;		// the data word
  sc_logic tag;		// tag logic bit
  sc_uint<AMCNT> dom;	// degree of match for k-nn hacks
};

// For writing out cam word values we overload the << operator 
inline ostream& operator << ( ostream& os, const cam_word& a )
{
  //os << "streaming of struct cam word not implemented \n";
  os << a.word << " Tag: " << a.tag  << " DoM: "<< a.dom << endl;
  return os;
}


// Associative Memory Module
// Note this is a pretty low level model and perhaps could be imlemented 
// trivialy with the C++ STL library functions, but that is not the point. 
// This implementation is closer to a CMOS implementation and hopfully we 
// can instrument it for timing, and/or synthesize it.
// We will have to replace it with a non-boolean (osilator based) 
// block in the future.

SC_MODULE(amem) {
  // IO ports
  sc_in<command_type> command;
  sc_in<am_word> compin; 	       // comparand
  sc_in<am_word> maskin; 	       // mask '0' is do care
  sc_in<am_word> writedata;	       // for writing new data
  sc_in<sc_uint<AMSZ> > address;       // address for ram read/write operations
  sc_in<bool> start;	       	       // start function
  sc_out<bool> done; 	       	       // completion signal
  sc_out<bool> match; 	               // any resonders
  sc_out<am_word> readdata;	       // for reading values
  sc_in_clk clock;

  // Internal structure 
  cam_word am_array[CAM_ARRAY_SIZE];   // the array of cam words
  void exe_command();		       // the main process for amem
 
  // Sub modules
  knn knn1; 			       // k nearest neighbor module
  friend class knn;

  // Signals
  sc_signal<sc_uint<AMCNT> > count, add;
  sc_signal<sc_uint<KN> > kn;
  sc_signal<bool> find_knn;

  // Constructor - note how sub modules are constructed
  SC_CTOR(amem) 
    : knn1("knn1")
    {
      // and wire up the sub module
      knn1.count(count);
      knn1.add(add);
      knn1.kn(kn);
      knn1.find(find_knn);

      // Register this function as a process to be scheduled
      SC_THREAD(exe_command);
      sensitive << clock.pos();
      dont_initialize(); // wait for first clock
    
      // Initialize Memory ?
      //  for(int i=0; i<CAM_ARRAY_SIZE; i++) {
      //  am_array[i].word = i;
      //  am_array[i].tag = 0;
      // }

    }
};

#endif
