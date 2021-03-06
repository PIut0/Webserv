#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <iostream>
#include <string>
#include <sys/time.h>

#include "Color.hpp"
#include "utils.hpp"

#include "HttpParseInvalidBody.hpp"
#include "HttpParseInvalidRequest.hpp"
#include "HttpParseInvalidResponse.hpp"
#include "HttpParseInvalidVersion.hpp"

#include "FdDupFailed.hpp"

class Monitor
{
 public:
  ~Monitor();
  Monitor();

  void Log(const std::string&);
  void Print(const std::string&, const std::string&);
  void Error();

  time_t getTimeStamp();
};

#endif
