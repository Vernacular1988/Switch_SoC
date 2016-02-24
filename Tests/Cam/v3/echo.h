//echo.h
#ifndef ECHOINC
#define ECHOINC

#include "systemc.h"
#include "cam.h"
#include "driver.h"

SC_MODULE(echo){
 sc_in<command_type> command;
 sc_out<sc_uint<4> > cmd;
 void echo_command();

 SC_CTOR(echo) {
   SC_METHOD(echo_command);
   sensitive << command ;
 }
};

#endif
