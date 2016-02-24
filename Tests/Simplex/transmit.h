// transmit.h
#include "packet.h"
SC_MODULE(transmit) {
	sc_in<packet_type> tpackin; // input port
	sc_in<bool> timeout; // input port
	sc_out<packet_type> tpackout; // output port
	sc_out<bool> start_timer; // output port  was inout
	sc_in<bool> clock;
	int buffer;
	int framenum;
	packet_type packin, tpackold;
	packet_type s;
	int retry;
	void send_data();
	int get_data_fromApp();

	// Constructor
	SC_CTOR(transmit) {
		SC_METHOD(send_data); // Method Process
		//sensitive << timeout;
		sensitive_pos << clock;
		framenum = 1;
		retry = 0;
		buffer = get_data_fromApp();
	}
};

