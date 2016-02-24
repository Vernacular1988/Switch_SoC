//draw.cc

#include "systemc.h"
#include "/usr/include/c++/3.3.4/string"
#include "/usr/include/c++/3.3.4/map"


std::map <std::string, int>bits_to_hex;

char *hex_bits[] = {"0000", "0001", "0010", "0011", 
		    "0100", "0101", "0110", "0111",
		    "1000", "1001", "1010", "1011",
		    "1100", "1101", "1110", "1111"};

void initialize_commands() {
  for(int i = 0; i< 16; i++) {
    bits_to_hex[hex_bits[i]] = i;
  }
}

void parse_error(char* s1, char* s2) {
  cerr << "ERROR "<< s1 << " " << s2 << endl;
  exit(1);
}

// very dumb fixed format line parser
// fields can be whitespace separated
// no comments
// numbers are assumed hex (0X is not needed/does not work)
//
long int parse_file(ifstream& fin) {
  std::string left, right;
  long int opint;
  long int word;

  //cout << "Note: data is taken as 2 binary strings" << endl;
  //cout << "Left \t Right" << endl;

  while (!fin.eof()) {
    fin >> left >> right;

    if ( fin.good() ) {

      word <<=6;
      opint = 0;

      //  cout << left << " " << right << endl;

      opint = bits_to_hex.find(left)->second;
      opint <<= 4;
      opint +=bits_to_hex.find(right)->second;

      //      cout << "HEX:" << hex << opint << dec << endl;

      word +=opint;

    }
  }
  //  cout << "Word: "<< word << " HEX: " << hex << word << dec << endl;


  return(word);
}


int main(int argc, char* argv[]) {
  long res = 0;

  if (argc == 1) {
    cout << "Number ?" << endl;
    cin >> hex >> res;
    cout << hex << res << dec << endl;

    for(int i = 0; i<4; i++) res<<=1;
    for(int i = 0; i<10; i++) {
      for(int j = 0; j<6; j++) {
	cout << ((res < 0) ? '*' : ' ') ;
	res <<= 1;
      }
      cout << endl;
    } 
    exit(0);
  }


  if (argc !=2 ) {
    parse_error("Wrong number of arguments","");
  }

  ifstream fin(argv[1]); 
  if(!fin) {
    parse_error("File open failed: ", argv[1]);
  }

 initialize_commands();
 
 cout << "Reading Program file: "<< argv[1] << endl;
 
 res = parse_file(fin);
 cout << hex << res << dec << endl;

 for(int i = 0; i<4; i++) res<<=1;
 for(int i = 0; i<10; i++) {
   for(int j = 0; j<6; j++) {
     cout << ((res < 0) ? '*' : ' ') ;
     res <<= 1;
   }
   cout << endl;
 }
}
