#include "System.hpp"

Monitor System::monitor;

Monitor& System::GetMonitor()
{
  return this->monitor;
}
