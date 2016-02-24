//knn.cc
#include "knn.h"
#define DEBUG 2

class amem;

void knn::find_knn() {
  //Method sensitive to changes in find
    sc_uint<AMCNT> cnt = count;
    sc_uint<KN> k = kn;
    neighbor temp;

  if(find) {
   for(int i = 0; i<CAM_ARRAY_SIZE; i++) {
     // See if we are better than worst nn to date
     if(amem::am_array[i].dom > neighbors[k-1].dom) {
       neighbors[k-1].dom = cnt;
       neighbors[k-1].add = add;
    
       // bubble up to correct postition in list
       for(int i = k-1; i>0; i--) {
	 if(neighbors[i].dom > neighbors[i-1].dom) {
	   temp = neighbors[i];
	   neighbors[i] = neighbors[i-1];
	   neighbors[i-1] = temp;
	 } // else break;
       } // for
     } //if
   } // for
  } // if(find)
}
