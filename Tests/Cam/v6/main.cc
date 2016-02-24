// main.cc
#include "cam.h"
#include "driver.h"
#include "echo.h"

int sc_main(int argc, char* argv[]) {

  sc_clock CLOCK("clock", 20, SC_NS);		// transmit clock
  sc_signal<am_word> comparand, mask, writedata, readdata;
  sc_signal<bool> mstart, mdone, mmatch;
  sc_signal<command_type> command;
  sc_signal<sc_uint<4> > cmd;			//how stupid is this
  sc_signal<sc_uint<AMSZ> > address;		// note space
	
  amem am1("am1");
  am1.command(command);
  am1.compin(comparand);
  am1.maskin(mask);
  am1.writedata(writedata);
  am1.address(address);
  am1.start(mstart);
  am1.done(mdone);
  am1.match(mmatch);
  am1.readdata(readdata);
  am1.clock(CLOCK);

  driver dr1("dr1");
  dr1.command(command);
  dr1.compout(comparand);
  dr1.maskout(mask);
  dr1.writedata(writedata);
  dr1.address(address);
  dr1.start(mstart);
  dr1.done(mdone);
  dr1.match(mmatch);
  dr1.readdata(readdata);
  dr1.clock(CLOCK);

//how stupid is this
  echo ec1("ec1");
  ec1.command(command);
  ec1.cmd(cmd);

  // tracing:
  // trace file creation
  sc_trace_file *tf = sc_create_vcd_trace_file ("cam");

  // External Signals
  sc_trace(tf, CLOCK.signal(), "clock");
  sc_trace(tf, cmd, "cmd");
  sc_trace(tf, comparand, "comp");
  sc_trace(tf, mask, "mask");
  sc_trace(tf, writedata, "writedata");
  sc_trace(tf, readdata, "readdata");
  sc_trace(tf, address, "address");
  sc_trace(tf, mstart, "start");
  sc_trace(tf, mdone, "done");
  sc_trace(tf, mmatch, "match");

  sc_start(2000, SC_NS);
  sc_close_vcd_trace_file(tf);
  return(0);
}

