// cam.cc
#include "cam.h"

void amem::compare() {
  bool any;

  am_word comp = compin;
  am_word mask = maskin;
  any = false;
  done = true;

  if (start && done) {

    done = false;
    //return; //next_trigger(); //next_trigger(clock->negedge_event());

  } else {
    if(!done) {
      for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {

	cout << am_array[i].word << endl<< comp << endl<< mask << endl<< endl;
       
	if (((am_array[i].word ^ comp) & ~mask) == 0) { 
	  am_array[i].tag = 1; 
	  any = true;
	  cout << "match at "<< i <<endl;
	} else {
	  am_array[i].tag = 0;
	  cout << "no match at "<< i <<endl;
	} 
      }
      done = true;
    }
  } //else
  match = any;
}
