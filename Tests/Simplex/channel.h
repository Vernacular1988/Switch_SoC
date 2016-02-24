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
