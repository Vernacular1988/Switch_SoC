// cam_array.cc
#include "cam_array.h"

void amem::compare() {
  
  bool any;

  am_word comp = compin;
  am_word mask = maskin;
  
  cout << am << endl;
       
	if (((am.word ^ comp) & ~mask) == 0) { 
	  am.tag = 1; 
	} else {
	  am.tag = 0;
	} 
}

void cam_array::compare() {

 m1->compare();
	// can I make a loop ?
	// or a macro ?



}
