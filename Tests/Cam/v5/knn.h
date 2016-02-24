//knn.h
#ifndef KNNINC
#define KNNINC

#include "systemc.h"
#include "constants.h"

// structure to keep track of the degree of match, DoM
// and location of a nearest neighbor
struct neighbor {
  sc_uint<AMCNT> dom;
  sc_uint<AMSZ>  add;
};

// sub-module used by amem to find the k best matches, after the 
// actual matching and counting of degree of match operations are done
// NOTE perhaps the counting should also be done in a sub module.
//
SC_MODULE(knn) {
  // IO ports
  sc_in<sc_uint<AMCNT> > count;	// DoM count in
  sc_in<sc_uint<AMCNT> > add;	// addrss of word
  sc_in<sc_uint<KN> > kn;	// k for knn (should be log(KN) bits)
  sc_in<bool> find;		// start finding

  // Internal Structure
  neighbor neighbors[KN];	// array of k nearest neighbors best at [0]
  void find_knn();		// only process for knn
		
  // Constructor
  SC_CTOR(knn) {
    SC_METHOD(find_knn);
    sensitive << find;		// use default signal event (change of signal)
    dont_initialize(); 		// wait for find
  }
};

#endif
