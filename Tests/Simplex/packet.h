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
//extern void sc_trace(sc_trace_file *tf, const packet_type& v, const sc_string& NAME);


inline
ostream&
operator << ( ostream& os, const packet_type& a )
{
    os << "streaming of struct pkt not implemented";
    return os;
}


#if defined(SC_API_VERSION_STRING)
extern void sc_trace( sc_trace_file* tf, const packet_type& v, const std::string& NAME );
#else
extern void sc_trace( sc_trace_file* tf, const packet_type& v, const sc_string& NAME );
#endif

#endif

