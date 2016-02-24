// timer.cc
#include "timer.h"
void timer::runtimer() {
	while (true) {
	  if (start || timeout) {
			cout <<"Timer: timer start detected"<<endl;
			count = 5; // need to make this a constant
			timeout = false;
		} else {
			if (count > 0) {
				count--;
				timeout = false;
				cout << "Timer: count "<<count<<"\n";
			} else {
				timeout = true;
				cout << "Timer: timeout! \n"; 
			}
		}
		wait();
	}
}
