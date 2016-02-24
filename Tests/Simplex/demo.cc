// packet.h file
#ifndef PACKETINC
#define PACKETINC
#include "systemc.h"

struct packet_type {
	long info;
	int seq;
	int retry;
	inline bool operator == (const packet_type& rhs) const {
		return (rhs.info == info && rhs.seq == seq && rhs.retry == retry);
	}
};
extern void sc_trace(sc_trace_file *tf, const packet_type& v,
					 const sc_string& NAME);
#endif

// packet.cc file
#include "packet.h"
void sc_trace(sc_trace_file *tf, const packet_type& v, const sc_string& NAME) {
	sc_trace(tf,v.info, NAME + ".info");
	sc_trace(tf,v.seq, NAME + ".seq");
	sc_trace(tf,v.retry, NAME + ".retry");
}


// transmit.h
#include "packet.h"
SC_MODULE(transmit) {
	sc_in<packet_type> tpackin; // input port
	sc_in<bool> timeout; // input port
	sc_out<packet_type> tpackout; // output port
	sc_inout<bool> start_timer; // output port
	sc_in<bool> clock;
	int buffer;
	int framenum;
	packet_type packin, tpackold;
	packet_type s;
	int retry;
	bool start;
	void send_data();
	int get_data_fromApp();

	// Constructor
	SC_CTOR(transmit) {
		SC_METHOD(send_data); // Method Process
		sensitive << timeout;
		sensitive_pos << clock;
		framenum = 1;
		retry = 0;
		start = false;
		buffer = get_data_fromApp();
	}
};

// transmit.cc
#include "transmit.h"
int transmit::get_data_fromApp() {
	int result;
		Transmit Module
		result = rand();
	cout <<"Generate:Sending Data Value = "<<result
		<< "\n";
	return result;
}
void transmit::send_data() {
	if (timeout) {
		s.info = buffer;
		s.seq = framenum;
		s.retry = retry;
		retry++;
		tpackout = s;
		start_timer = true;
		cout <<"Transmit:Sending packet no. "<<s.seq
			<< "\n";
	} else {
		packin = tpackin;
		if (!(packin == tpackold)) {
			if (packin.seq == framenum) {
				buffer = get_data_fromApp();
				framenum++;
				retry = 0;
			}
			tpackold = tpackin;
			s.info = buffer;
			s.seq = framenum;
			s.retry = retry;
			retry++;
			tpackout = s;
			start_timer = true;
			cout <<"Transmit:Sending packet no. "<<s.seq
				<< "\n";
		}
	}
}

// channel.h
#include "packet.h"
SC_MODULE(channel) {
	sc_in<packet_type> tpackin; // input port
	sc_in<packet_type> rpackin; // input port
	sc_out<packet_type> tpackout; // output port
	sc_out<packet_type> rpackout; // output port
	packet_type packin;
	packet_type packout;
	packet_type ackin;
	packet_type ackout;
	void receive_data();
	void send_ack();

	// Constructor
	SC_CTOR(channel) {
		SC_METHOD(receive_data); // Method Process
		sensitive << tpackin;
		SC_METHOD(send_ack); // Method Process
		sensitive << rpackin;
	}
};
// channel.cc
#include "channel.h"
void channel::receive_data() {
	int i;

	packin = tpackin;
	cout << "Channel:Received packet seq no.
		= " << packin.seq << "\n";
		i = rand();
	packout = packin;
	cout <<"Channel: Random number = "<<i<<endl;
	if ((i > 1000) && (i < 5000)) {
		packout.seq = 0;
	}
	rpackout = packout;
}
void channel::send_ack(){
	int i;

	ackin = rpackin;
	cout <<"Channel:Received Ack for packet
		Starting with a Simple Example
		24 SystemC 2.0 User’s Guide
		= " << ackin.seq << "\n";
		i = rand();
	ackout = ackin;
	if ((i > 10) && (i < 500)) {
		ackout.seq = 0;
	}
	tpackout = ackout;
}

// receiver.h
#include "packet.h"
SC_MODULE(receiver) {
	sc_in<packet_type> rpackin; // input port
	sc_out<packet_type> rpackout; // output port
	sc_out<long> dout; // output port
	sc_in<bool> rclk;
	int framenum;
	packet_type packin, packold;
	packet_type s;
	int retry;
	void receive_data();

	// Constructor
	SC_CTOR(receiver) {
		SC_METHOD(receive_data); // Method Process
		sensitive_pos << rclk;
		framenum = 1;
		retry = 1;
	}
};
// receiver.cc
#include "receiver.h"
void receiver::receive_data(){

	packin = rpackin;

	if (packin == packold) return;

	cout <<"Receiver: got packet no. = "<<packin.seq
		<< "\n";
	if (packin.seq == framenum) {
		dout = packin.info;
		framenum++;
		retry++;
		s.retry = retry;
		s.seq = framenum -1;
		rpackout = s;
	}
	packold = packin;
}

// display.h
#include "systemc.h"
#include "packet.h"
SC_MODULE(display) {
	sc_in<long> din; // input port
	void print_data();

	// Constructor
	SC_CTOR(display) {
		SC_METHOD(print_data); // Method process to print data
			sensitive << din;
	}
};
// display.cc
#include "display.h"
void display::print_data() {
	cout <<"Display:Data Value Received, Data = "
		<<din << "\n";
}

// timer.h
#include "systemc.h"
SC_MODULE(timer) {
	sc_inout<bool> start; // input port
	sc_out<bool> timeout; // output port
	sc_in<bool> clock; // input port
	int count;
	void runtimer();

	// Constructor
	SC_CTOR(timer) {
		SC_THREAD(runtimer); // Thread process
		sensitive_pos << clock;
		sensitive << start;
		count = 0;
	}
};

// timer.cc
#include "timer.h"
void timer::runtimer() {
	while (true) {
		if (start) {
			cout <<"Timer: timer start detected"<<endl;
			count = 5; // need to make this a constant
			timeout = false;
			start = false;
		} else {
			if (count > 0) {
				count--;
				timeout = false;
			} else {
				timeout = true;
			}
		}
		wait();
	}
}


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
	sc_clock CLOCK("clock", 20); // transmit clock
	sc_clock RCLK("rclk", 15); // receive clock

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

	sc_start(10000);
	sc_close_vcd_trace_file(tf);
	return(0);
}


//Makefile
MODULE = demo
SRCS = channel.cc display.cc packet.cc receiver.cc timer.cc transmit.cc main.cc
OBJS = $(SRCS:.cc=.o)
include ./Makefile.defs


//Makefile.defs
TARGET_ARCH = gccsparcOS5
CC = g++
OPT =
DEBUG = -g
SYSTEMC = /remote/dtg403/dperry/systemc-2.0
INCDIR = -I. -I.. -I$(SYSTEMC)/include
LIBDIR = -L. -L.. -L$(SYSTEMC)/lib-$(TARGET_ARCH)
CFLAGS = -Wall $(DEBUG) $(OPT) $(INCDIR) $(LIBDIR)
LIBS = -lsystemc -lm $(EXTRA_LIBS)
