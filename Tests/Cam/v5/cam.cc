// cam.cc
#include "cam.h"


// 0 1 or 2
#define DEBUG 2

void amem::exe_command() {

  // We need to make copies of all the inputs to allow us to do math/logic
  // Note: One can do math on sc_ types but not sc_signals of those same types
  am_word comp;
  am_word mask;
  am_word wdata;
  am_word rdata;
  sc_uint<AMSZ>  add;
  command_type cmd;
  bool any;	// Array deep "or" for match operation
  
  done = true; // Initalize as ready to receive instructions

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

      // Tell the world you are working, and wait for the next clock
      done = false;
      match = false;

      if (DEBUG) cout << "AM Command is: " << cmd << endl;

      wait();

      // Now do the work based on the command op code
      switch (cmd) {

	/////////////////////////////////////////////////////////

	// NOTE Compare and Hamming are NOT enabled by TAG.
	// Probably we do want it enabled by TAG
	// as IS TRUE for AREAD and AWRITE.
	// This would allow for concatinated seaches.
	// But then we would need to do a SETTAG operation 
	// before starting most searches.

	// Any (match) should be computed all the time.
	// (it would be a big OR gate).
	// And we should add count responders.

      case COMPARE: // Compare with broadcast comparand and mask
	// mask[i] == 1 means ignore bit i of comparand
	if(DEBUG) cout << "Compare Operation" << endl;

	any = false;

	for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {

	  if (((am_array[i].word ^ comp) & ~mask) == 0) { 
	    am_array[i].tag = 1; 
	    any = true;
	    if(DEBUG>1) cout << "match at "<< i <<endl;
	  } else {
	    am_array[i].tag = 0;
	    if(DEBUG>1) cout << "no match at "<< i <<endl;
	  } 
	  if(DEBUG>1) cout << am_array[i] << comp << endl<< mask << endl << endl;
	}
	match = any;
	break;

	/////////////////////////////////////////////////////////

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

	/////////////////////////////////////////////////////////

      case AWRITE: //Associative Write
	// Writes to ALL words with tag set, masked by mask
	if(DEBUG) cout << "Write Operation" << endl;

	for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {
	  if(am_array[i].tag == '1') {
	    am_array[i].word = ((am_array[i].word & mask) | (wdata & ~mask));
	  }

	  if(DEBUG>1) cout << am_array[i] << endl;
	}
	break;

	/////////////////////////////////////////////////////////

      case READ: // RAM Read
	// Reads from one word at address
	if(DEBUG) cout << "Read Operation" << endl;

	rdata = am_array[add].word;

	if(DEBUG>1)
	  for(int i = 0 ; i<CAM_ARRAY_SIZE; i++)
	    cout << am_array[i] << endl;

	readdata = rdata;
	break;

	/////////////////////////////////////////////////////////

      case WRITE: // RAM Write
	// Writes to one word at address, masked by mask
	if(DEBUG) cout << "Write Operation" << endl;

	am_array[add].word = ((am_array[add].word & mask) | (wdata & ~mask));

	if(DEBUG>1)
	  for(int i = 0 ; i<CAM_ARRAY_SIZE; i++)
	    cout << am_array[i] << endl;
	break;

	/////////////////////////////////////////////////////////

      case SETTAG: // Set all Tags
	if(DEBUG) cout << "Set All Tags" << endl;

	for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {
	  am_array[i].tag = '1';
	  if(DEBUG>1) cout << am_array[i] << endl;
	}
	break;

	/////////////////////////////////////////////////////////

      case CLEARTAG: // Clear all Tags and Degree of Match counters
	if(DEBUG) cout << "Clear All Tags" << endl;

	for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {
	  am_array[i].tag = '0';
	  am_array[i].dom = 0;
	  if(DEBUG>1) cout << am_array[i] << endl;
	}
	break;

	/////////////////////////////////////////////////////////

      case COMPTAG: // Clear all Tags
	if(DEBUG) cout << "Complement All Tags" << endl;

	for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {
	  am_array[i].tag = ~am_array[i].tag;
	  if(DEBUG>1) cout << am_array[i] << endl;
	}
	break;

	/////////////////////////////////////////////////////////

      case SELFIRST: // Select the First Tag (pick one)
	if(DEBUG) cout << "Select First Tag" << endl;
	{
	  bool found = 0;
	  for(int i = 0 ; i<CAM_ARRAY_SIZE; i++) {
	    if(am_array[i].tag == 1) {
	      // if this is not the first, reset it
	      if (found) am_array[i].tag = '0';
	      // if it was the first, remember we found one
	      found = 1; 
	    }
	    if(DEBUG>1) cout << am_array[i] << endl;
	  }
	}
	break;

	/////////////////////////////////////////////////////////

      case HAMMING: // Calculate Hamming Distance in DoM counters
	if(DEBUG) cout << "Calculate Hammng Distance" << endl;

	// Loop over words and then bits in each word to count bit matches
	for(int i = 0; i<CAM_ARRAY_SIZE; i++) {
	  for(int j = 0; j<CAM_WORD_WIDTH; j++) { 
	    if (((am_array[i].word[j] ^ comp[j]) & ~mask[j]) == 0) { 
	      am_array[i].dom++; 
	    }
	  }
	  if(DEBUG>1) cout << am_array[i] << endl;
	}
	break;

	/////////////////////////////////////////////////////////

      case WTA: // Winner Take All (find largest DoM counter) 
	if(DEBUG) cout << "Winner take all" << endl;

	// Loop over words and set tag on first best matching word
	// Word can be read out with AREAD
	// Do we want to output the DoM value itself?
	{  	
	  int best = 0;
	  bool found = 0;

	  for(int i = 0; i<CAM_ARRAY_SIZE; i++) {
	    if (am_array[i].dom > best) {
	      best = am_array[i].dom;
	    }
	  }
	  // Have to set tag on second pass to avoid local max problem
	  for(int i = 0; i<CAM_ARRAY_SIZE; i++) {
	    if ((am_array[i].dom == best) && !found) {
	      am_array[i].tag = '1';
	      found = '1';
	    } else {
	      am_array[i].tag = '0';
	    }
	    if(DEBUG>1) cout << am_array[i] << endl;
	  }
	}
	break;

	/////////////////////////////////////////////////////////

      case KNN: //find k nearest neighbors based on counts not just best
	if(DEBUG) cout << "Find K nearest neigbors" << endl;
	find_knn = true;
	wait();
	find_knn = false;
	break;

	/////////////////////////////////////////////////////////

      default: // Undefined command
	if(DEBUG) cout << "Error bad command" << endl;
	break;
	
      } // end of switch

      // tell the world you are done
      done = true;

    } // end of if(start && done) 

    wait(); // must always be one wait in an infinite loop
  } // end of loop 
}
