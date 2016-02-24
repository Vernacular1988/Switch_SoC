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

