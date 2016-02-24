// driver.cc

#include "driver.h"

// 0 1 or 2
#define DEBUG 2


// very dumb fixed format line parser
// fields can be whitespace separated
// no comments
// numbers are assumed hex (0X is not needed/does not work)
//
int driver::set_next_command() {

  std::string op;
  long int opint, maskint, compint, dataint, addrint;

  if(!fin.eof()) {
    fin >> op >> hex >> maskint >> compint >> dataint >> addrint;

    if ( fin.good() ) {
 
      opint = str_to_cmd.find(op)->second;

      if(DEBUG) {
	cout << "OPCODE \t MASK \t COMP \t DATA \t ADDR" << endl;
	cout << op <<" \t " << hex << maskint <<" \t " << compint 
	   <<" \t " << dataint <<" \t " << addrint << dec << endl;
      } 

    com.op =  (command_type) opint;
    com.msk = maskint;
    com.cmp = compint;
    com.wd = dataint;
    com.ad = addrint;
    return(1);
    }
  }
  return(0);
}



void driver::rundriver() {
  compout = 0;
  maskout = 0;
  int active = 1;

  // Run continuously/synchronously while there are commands to do
  while (active) {
    // see if AM has finished current command;
    if(done) {
      if(DEBUG) cout << "Driver command number: "<< count << endl;
      active = set_next_command();
      if(DEBUG)   cout << "Driver command is: "<< com.op << endl;
      if(DEBUG>1) cout << com.msk << "\tMask" << endl
		       << com.cmp << "\tComparand" << endl
		       << com.wd  << "\tWrite Data" << endl
		       << "\t\t\t\t" << com.ad << "\tAddress" << endl;
      command = com.op;
      maskout = com.msk;
      compout = com.cmp;
      writedata = com.wd;
      address = com.ad;
	 
      start = true;
      wait();
      start = false;
      //wait();
      count++;
    }
    wait(); // synchronosly wait for for AM ready (done)
  }
}
