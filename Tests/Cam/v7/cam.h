//cam.h
#ifndef CAMINC
#define CAMINC

#include "constants.h"

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

// structure to keep track of the degree of match, DoM
// and location of a nearest neighbor
struct neighbor {
  sc_uint<AMCNT> dom;	// copy of the degree of match for that word
  sc_uint<AMSZ>  add;	// address of word that holds that match
};

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
  cam_word am_array[CAM_ARRAY_SIZE];   	// the array of cam words
  void exe_command();			// the main process for amem

  // Constructor - note how sub modules are constructed
  SC_CTOR(amem) 
    {
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
