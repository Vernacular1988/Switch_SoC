// display.h
#ifndef DISPLAYINC
#define DISPLAYINC

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
#endif
