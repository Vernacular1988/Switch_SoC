// cam.cc
#include "cam.h"

void amem::exe_command() {

  // not sure if we need to make copies of all these inputs

  am_word comp = compin;
  am_word mask = maskin;
  am_word wdata = writedata;
  am_word rdata = readdata;
  sc_uint<AMSZ>  add = address;
  command_type cmd = command;
  
  bool any;


  done = true;			// no copy of output

  if (start && done) {
    // tell the world you are working and wait for the next clock
    done = false;
    match = false;
    cout << "CAM Command is: " << cmd << endl;

    return; //or next_trigger(); //or try next_trigger(clock->negedge_event());

  } else {
    if(!done) {
      // do the work 
      switch (cmd) {
      
      case COMPARE:
	any = false;
	cout << "Compare Operation" << endl;

	for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {

	  cout << am_array[i] << comp << endl<< mask << endl;
       
	  if (((am_array[i].word ^ comp) & ~mask) == 0) { 
	    am_array[i].tag = 1; 
	    any = true;
	    cout << "match at "<< i <<endl;
	  } else {
	    am_array[i].tag = 0;
	    cout << "no match at "<< i <<endl;
	  } 
	  cout << endl;
	}
	match = any;
	break;

      case WRITE: 
	  // writes to ALL words with tag set, masked by mask
	cout << "Write Operation" << endl;
	    am_array[add].word = (wdata & ~mask);

	for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {

	  //if(am_array[i].tag == '1') {
	  //}

	  cout << am_array[i].word << endl<< endl;
	}
	break;

      case READ:
 
	cout << "Read peration" << endl;

	for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {
	  if(am_array[i].tag == '1') {
	    // reads from last word with tag set
	    rdata = am_array[i].word;
	  }

	  cout << am_array[i].word << endl<< endl;
	}
	break;

      default: 
	cout << "Error bad command" << endl;
	break;
	
      } // end switch
      done = true;
    }
  }
}
