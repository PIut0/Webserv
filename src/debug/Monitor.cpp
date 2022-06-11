#include "Monitor.hpp"

Monitor::Monitor()
{
    this->logfile_.open("./log/log.txt");
}

Monitor::~Monitor()
{
  this->logfile_.close();
}

time_t Monitor::getTimeStamp() {
    struct timeval time_now;
    gettimeofday(&time_now, nullptr);

    time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
    return msecs_time;
}

void Monitor::Log(const std::string &data)
{
  std::string time = std::to_string(this->getTimeStamp());
  std::string _log = time + " " + "Scope" + " " + data + "\n";
  
  this->logfile_.write(_log.c_str(), _log.length());
}

void Monitor::Print(const std::string &color, const std::string &data)
{
  std::cout << color << data << COLOR_DEFAULT << std::endl;
}
