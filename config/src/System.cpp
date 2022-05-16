#include "System.hpp"

Monitor System::monitor;

Monitor& System::getMonitor()
{
	return this->monitor;
}
