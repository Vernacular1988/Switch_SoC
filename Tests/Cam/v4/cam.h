//cam.h
#ifndef CAMINC
#define CAMINC

#include "systemc.h"

// Define cam array size parameters 
#define CAM_WORD_WIDTH 32      // binary bit width
#define AMCNT 6		       // bit counter size (log width +1) 
#define CAM_ARRAY_SIZE 16      // words per cluster array
#define AMSZ 4                 // address bits (log size)


// Commands for the controller, note: ERR is in position zero for null command
enum command_type {ERR, COMPARE, AREAD, AWRITE, READ, 
		   WRITE, SETTAG, CLEARTAG, COMPTAG, 
		   SELFIRST, HAMMING, WTA};

// For writing out command values we overload the << operator 
inline ostream& operator << ( ostream& os, const command_type& t )
{
  //MOVE THIS OUT OF FUNCTION
  char *command_string[] = {"ERR", "COMPARE", "AREAD", "AWRITE", "READ", 
			    "WRITE", "SETTAG", "CLEARTAG", "COMPTAG",
			    "SELFIRST", "HAMMING", "WTA"}; 
  os << command_string[t];
  return os;
}

// Definitions for each word for a binary cam implementation
// Note we are using logic vectors (vs bits) which are slower
// but can have X and Z values

typedef sc_lv<CAM_WORD_WIDTH> am_word;

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
// Note this is a pretty low level model and perhaps could be imlemented trivialy with 
// the C++ STL library functions, but that is not the point. This implementation is
// closer to a CMOS implementation and hopfully we can instrument it for timing, and/or synthesize it.
// We will have to replace it with a non-boolean (osilator based) block in the future.

SC_MODULE(amem) {
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
  cam_word am_array[CAM_ARRAY_SIZE];   // the array of cam words
  void exe_command();

  // Constructor
  SC_CTOR(amem) {
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
