#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <iostream>
#include <fstream>
#include <sys/time.h>

#include "Monitor.hpp"

typedef std::chrono::high_resolution_clock clock_cro_t;
typedef std::chrono::duration<float, std::milli> mil;

#define __CLOCK__(A) (A = clock_cro_t::now());

#define CLOCK_START(A)  __CLOCK__(A)
#define CLOCK_END(A)    __CLOCK__(A)
#define CLOCK_PRINT(C, A)  (C->printClock(A));

class System
{
 public:
  Monitor& GetMonitor();

  std::__1::chrono::steady_clock::time_point t0, t1;

  void printClock(const std::string &origin) {
    std::cout << origin << " : " << mil(t1 - t0).count() << "ms\n" << std::endl;
  }

 private:
  static Monitor monitor_;
};

#endif
