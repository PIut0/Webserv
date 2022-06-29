#include "Monitor.hpp"

Monitor::Monitor() {}

Monitor::~Monitor() {}

time_t Monitor::getTimeStamp() {
  struct timeval time_now;
  gettimeofday(&time_now, nullptr);

  time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);

  return msecs_time;
}

void Monitor::Log(const std::string &data)
{
  std::string _log = " Scope " + data + "\n";

  std::cout << this->getTimeStamp() << _log.c_str() << std::endl;
}

void Monitor::Print(const std::string &color, const std::string &data)
{
  std::cout << color << data << COLOR_DEFAULT << std::endl;
}
