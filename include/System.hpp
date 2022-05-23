#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <iostream>
#include <fstream>
#include <sys/time.h>

#include "Monitor.hpp"

class System
{
public:
  Monitor& GetMonitor();

private:
  static Monitor monitor;
};

#endif
