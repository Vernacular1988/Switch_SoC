// cam.cc
#include "cam.h"

#define DEBUG 1


void amem::exe_command() {

  // We need to make copies of all the inputs to allow us to do math/logic
  // Note: One can do math on sc_ types but not sc_signals of those same types
  am_word comp;
  am_word mask;
  am_word wdata;
  am_word rdata;
  sc_uint<AMSZ>  add;
  command_type cmd;
  bool any;	// Array wide "or" for match operation
  
  done = true; // initalize as ready to receive instructions

  // Loop forever waiting for commands 
  while(true) {
    comp = compin;	// Make copies of signals for logical ops
    mask = maskin;
    wdata = writedata;
    rdata = readdata;
    add = address;
    cmd = command;
 
    // See if we can accept a command, and if we have one to work on
    if (start && done) {
      // Tell the world you are working and wait for the next clock
      done = false;
      match = false;

      if (DEBUG) cout << "CAM Command is: " << cmd << endl;

      wait();

      // Now do the work based on the command op code

      switch (cmd) {
      
      case COMPARE: // Compare with broadcast comparand and mask
	      // mask[i] == 1 means ignore bit i of comparand
	if(DEBUG) cout << "Compare Operation" << endl;

	any = false;

	for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {

	  if (((am_array[i].word ^ comp) & ~mask) == 0) { 
	    am_array[i].tag = 1; 
	    any = true;
	    if(DEBUG) cout << "match at "<< i <<endl;
	  } else {
	    am_array[i].tag = 0;
	    if(DEBUG) cout << "no match at "<< i <<endl;
	  } 
	  if(DEBUG) cout << am_array[i] << comp << endl<< mask << endl << endl;
	}
	match = any;
	break;

      case AREAD: // Associative Read 
	      // Reads from last word with tag set (could be OR of words)
	if(DEBUG) cout << "Read operation" << endl;

	for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {
	  if(am_array[i].tag == '1') {
	    rdata = am_array[i].word;
	  }
	  if(DEBUG) cout << am_array[i] << rdata << " was read " << endl << endl;
	}
	readdata = rdata;
	break;

      case AWRITE: //Associative Write
	      // Writes to ALL words with tag set, masked by mask
	if(DEBUG) cout << "Write Operation" << endl;

	for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {
	  if(am_array[i].tag == '1') {
	    am_array[i].word = ((am_array[i].word & mask) | (wdata & ~mask));
	  }

	  if(DEBUG) cout << am_array[i] << endl;
	}
	break;

      case READ: // RAM Read
	      // Reads from one word at address
	if(DEBUG) cout << "Read Operation" << endl;

	rdata = am_array[add].word;

	if(DEBUG)
	  for(int i = 0 ; i<CAM_ARRAY_SIZE; i++)
	    cout << am_array[i] << endl;

	readdata = rdata;
	break;

      case WRITE: // RAM Write
	      // Writes to one word at address, masked by mask
	if(DEBUG) cout << "Write Operation" << endl;

	am_array[add].word = ((am_array[add].word & mask) | (wdata & ~mask));

	if(DEBUG)
	  for(int i = 0 ; i<CAM_ARRAY_SIZE; i++)
	    cout << am_array[i] << endl;
	break;

      default: // Undefined command
	if(DEBUG) cout << "Error bad command" << endl;
	break;
	
      } // end of switch

      // tell the world you are done
      done = true;

    } // end of if (start and done) 

    wait(); // must always be one wait in an infinite loop
  }
}
