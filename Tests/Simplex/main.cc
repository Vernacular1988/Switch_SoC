// main.cc
#include "packet.h"
#include "timer.h"
#include "transmit.h"
#include "channel.h"
#include "receiver.h"
#include "display.h"
int sc_main(int argc, char* argv[]) {
  sc_signal<packet_type> PACKET1, PACKET2, PACKET3, PACKET4;
  sc_signal<long> DOUT;
  sc_signal<bool> TIMEOUT, START;
  sc_clock CLOCK("clock", 20, SC_NS); // transmit clock
  sc_clock RCLK("rclk", 15, SC_NS); // receive clock

	transmit t1("transmit");
	t1.tpackin(PACKET2);
	t1.timeout(TIMEOUT);
	t1.tpackout(PACKET1);
	t1.start_timer(START);
	t1.clock(CLOCK);
	
	channel c1("channel");
	c1.tpackin(PACKET1);
	c1.rpackin(PACKET3);
	c1.tpackout(PACKET2);
	c1.rpackout(PACKET4);
	
	receiver r1("receiver");
	r1.rpackin(PACKET4);
	r1.rpackout(PACKET3);
	r1.dout(DOUT);
	r1.rclk(RCLK);
	
	display d1("display");
	d1 <<DOUT;
	
	timer tm1("timer");
	tm1 <<START<<TIMEOUT<<CLOCK.signal();

	// tracing:
	// trace file creation
	sc_trace_file *tf = sc_create_vcd_trace_file ("simplex");

	// External Signals
	sc_trace(tf, CLOCK.signal(), "clock");
	sc_trace(tf, TIMEOUT, "timeout");
	sc_trace(tf, START, "start");
	sc_trace(tf, PACKET1, "packet1");
	sc_trace(tf, PACKET2, "packet2");
	sc_trace(tf, PACKET3, "packet3");
	sc_trace(tf, PACKET4, "packet4");

	sc_trace(tf, DOUT, "dout");

	sc_start(10000, SC_NS);
	sc_close_vcd_trace_file(tf);
	return(0);
}

