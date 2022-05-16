#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <iostream>
#include <fstream>
#include <sys/time.h>

#include "Monitor.hpp"

class System
{
public:
	Monitor& getMonitor();

private:
	static Monitor monitor;
};

#endif
