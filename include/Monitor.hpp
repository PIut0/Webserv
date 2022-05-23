#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>

#include "Color.hpp"
#include "utils.hpp"


class Monitor
{
public:
  time_t getTimeStamp();
  void log(const std::string&);
  void print(const std::string&, const std::string&);
  void error();

  ~Monitor();
  Monitor();
private:

  std::ofstream logfile;
  int _seek;
};

#endif
