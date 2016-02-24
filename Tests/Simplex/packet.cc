
// packet.cc file
#include "packet.h"

//void sc_trace(sc_trace_file *tf, const packet_type& v, const sc_string& NAME) 
#if defined(SC_API_VERSION_STRING)
extern void sc_trace( sc_trace_file* tf, const packet_type& v, const std::string& NAME )
#else
extern void sc_trace( sc_trace_file* tf, const packet_type& v, const sc_string& NAME )
#endif

{
	sc_trace(tf,v.info, NAME + ".info");
	sc_trace(tf,v.seq, NAME + ".seq");
	sc_trace(tf,v.retry, NAME + ".retry");
}

