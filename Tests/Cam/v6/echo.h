//echo.h
#ifndef ECHOINC
#define ECHOINC

#include "systemc.h"
#include "constants.h"


SC_MODULE(echo){
 sc_in<command_type> command;
 sc_out<sc_uint<4> > cmd; // 16 commands
 void echo_command();

 SC_CTOR(echo) {
   SC_METHOD(echo_command);
   sensitive << command ;
 }
};

#endif
