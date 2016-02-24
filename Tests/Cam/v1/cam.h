//cam.h
#ifndef CAMINC
#define CAMINC

#include "systemc.h"

#define CAM_WORD_WIDTH 64
#define CAM_ARRAY_SIZE 10

typedef sc_lv<CAM_WORD_WIDTH> am_word;

struct cam_word {
  am_word word;
  sc_logic tag;
};

//for writing out cam word values
inline
ostream&
operator << ( ostream& os, const cam_word& a )
{
  //os << "streaming of struct cam word not implemented \n";
  os << a.word << " Tag: " << a.tag << endl;
  return os;
}


SC_MODULE(amem) {
  sc_in<am_word> compin; // comparand
  sc_in<am_word> maskin; // mask '0' is don't care
  sc_in<bool> start;   // start funtion
  sc_out<bool> done;   // completion signal
  sc_out<bool> match;  // any resonders

  // add count and best / wta

  sc_in_clk clock;

  cam_word am_array[CAM_ARRAY_SIZE]; // should we make this dynamic ?

  void compare();  // perhaps a command with an enumerated type is better


  // Constructor
  SC_CTOR(amem) {
    // Method Process
    SC_METHOD(compare);
    sensitive << clock.pos();

    // Initialize Memory ?
    for(int i=0; i<CAM_ARRAY_SIZE; i++) {
      am_array[i].word = i;
      am_array[i].tag = 0;
    }
  }  
};

#endif
