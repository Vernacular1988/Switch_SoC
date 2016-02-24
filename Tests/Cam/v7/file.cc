//file.cc

#include "systemc.h"
#include "/usr/include/c++/3.3.4/string"
#include "/usr/include/c++/3.3.4/map"
#include "constants.h"

std::map <std::string, int>str_to_cmd;

char *commands_string[] = {"ERR", "COMP", "AREAD", "AWRITE", 
			    "READ", "WRITE", "STAG", "CTAG", 
			    "ITAG", "SEL1ST", "HAMM", "WTA", 
                            "READNB"};
void initialize_commands() {
  for(int i = ERR; i<= READNB; i++) {
    str_to_cmd[commands_string[i]] = i;
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
void parse_file(ifstream& fin) {
  std::string op;
  int opint, maskint, compint, dataint, addrint;

  cout << "Note: data and address values are taken as hex" << endl;
  cout << "OPCODE \t MASK \t COMP \t DATA \t ADDR" << endl;

  while (!fin.eof()) {
    fin >> op >> hex >> maskint >> compint >> dataint >> addrint;

    if ( fin.good() ) {
      cout << op <<" \t " << hex << maskint <<" \t " << compint 
	   <<" \t " << dataint <<" \t " << addrint << dec << endl;

      opint = str_to_cmd.find(op)->second;

      cout << "Command:" << op <<" = "<< hex 
	   << str_to_cmd.find(op)->second 
	   << " = " << commands_string[opint] << dec << endl; 
    }
  }
}


int main(int argc, char* argv[]) {
  if (argc !=2 ) {
    parse_error("Wrong number of arguments","");
  }

  ifstream fin(argv[1]); 
  if(!fin) {
    parse_error("File open failed: ", argv[1]);
  }

 initialize_commands();
 
 cout << "Reading Program file: "<< argv[1] << endl;
 
 parse_file(fin);

}
