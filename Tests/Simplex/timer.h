// timer.h
#include "systemc.h"
SC_MODULE(timer) {
	sc_in<bool> start; // input port  was inout
	sc_out<bool> timeout; // output port
	sc_in<bool> clock; // input port
	int count;
	void runtimer();

	// Constructor
	SC_CTOR(timer) {
		SC_THREAD(runtimer); // Thread process
		sensitive_pos << clock;
		//sensitive << start;
		count = 0;
	}
};
