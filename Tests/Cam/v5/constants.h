// constants.h
// has defined constants for all files
#ifndef CONSTINC
#define CONSTINC

// Define cam array size parameters 
#define CAM_WORD_WIDTH 32      // binary bit width
#define AMCNT 6		       // bit counter size (log width +1) 
#define CAM_ARRAY_SIZE 16      // words per cluster array
#define AMSZ 4                 // address bits (log size)

// Commands for the controller, note: ERR is in position zero for null command
enum command_type {ERR, COMPARE, AREAD, AWRITE, 
		   READ, WRITE, SETTAG, CLEARTAG, 
		   COMPTAG, SELFIRST, HAMMING, WTA, 
		   KNN};

// For writing out command values we overload the << operator 
inline ostream& operator << ( ostream& os, const command_type& t )
{
  //MOVE THIS OUT OF FUNCTION
  char *command_string[] = {"ERR", "COMPARE", "AREAD", "AWRITE", 
			    "READ", "WRITE", "SETTAG", "CLEARTAG", 
			    "COMPTAG", "SELFIRST", "HAMMING", "WTA", 
			    "KNN"};
 
  os << command_string[t];
  return os;
}

// Definition for each word for a binary cam implementation
// Note we are using logic vectors (vs bits) which are slower
// but can have X and Z values

typedef sc_lv<CAM_WORD_WIDTH> am_word;


#define KN 8			// max number of nearest neighbors

#endif
