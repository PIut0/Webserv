#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>

#include "Color.hpp"
#include "utils.hpp"

#include "HttpParseInvalidBody.hpp"
#include "HttpParseInvalidMethod.hpp"
#include "HttpParseInvalidRequest.hpp"
#include "HttpParseInvalidResponse.hpp"

#include "NotFoundError.hpp"
#include "ForbiddenError.hpp"
#include "InternalServerError.hpp"

class Monitor
{
 public:
  ~Monitor();
  Monitor();

  void Log(const std::string&);
  void Print(const std::string&, const std::string&);
  void Error();

  time_t getTimeStamp();

 private:
  std::ofstream logfile_;
  int           seek_;
};

#endif
