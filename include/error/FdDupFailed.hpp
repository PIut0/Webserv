#ifndef FDDUPFAILED_HPP
#define FDDUPFAILED_HPP

#include <exception>

class FdDupFailed : public std::exception
{
 public:
	const char * what () const throw () {
    return "FdDupFailed";
  };
};

#endif
