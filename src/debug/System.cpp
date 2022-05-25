#include "System.hpp"

Monitor System::monitor_;

Monitor& System::GetMonitor()
{
  return this->monitor_;
}
