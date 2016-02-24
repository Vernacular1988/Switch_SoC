// echo.cc
#include "echo.h"
#define DEBUG 0

// how stupid is this? Just to convert from an enumerated type to a uint

void echo::echo_command() {
  command_type c1;
  cmd = c1 = command;
}

