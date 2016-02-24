// constants.h
// has defined constants for all files
#ifndef CONSTINC
#define CONSTINC

#include "/usr/include/c++/3.3.4/string"
#include "/usr/include/c++/3.3.4/map"
#include "systemc.h"

// Define cam array size parameters 
#define CAM_WORD_WIDTH 64      // binary bit width
#define AMCNT 7		       // bit counter size (log width +1) 
#define CAM_ARRAY_SIZE 32      // words per cluster array
#define AMSZ 5                 // address bits (log size)

extern std::map <std::string, int>str_to_cmd;
extern char *commands_string[] ;
extern ifstream fin;

// Commands for the controller, note: ERR is in position zero for null command
enum command_type {ERR, COMP, AREAD, AWRITE, 
		   READ, WRITE, STAG, CTAG, 
		   ITAG, SEL1ST, HAMM, WTA, 
		   READNB};

// For writing out command values we overload the << operator 
inline ostream& operator << ( ostream& os, const command_type& t )
{
  //MOVE THIS OUT OF FUNCTION
  //  char *command_string[] = {"ERR", "COMP", "AREAD", "AWRITE", 
  //			    "READ", "WRITE", "STAG", "CTAG", 
  //			    "ITAG", "SEL1ST", "HAMM", "WTA", 
  //                         "READNB"};
  //
  os << commands_string[t];
  return os;
}

// Definition for each word for a binary cam implementation
// Note we are using logic vectors (vs bits) which are slower
// but can have X and Z values

typedef sc_lv<CAM_WORD_WIDTH> am_word;


#define KN 8			// max number of nearest neighbors



#endif
