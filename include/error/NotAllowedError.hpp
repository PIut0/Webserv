#ifndef NOTALLOWEDERROR_HPP
#define NOTALLOWEDERROR_HPP

#include <exception>

class NotAllowedError : public std::exception
{
 public:
	const char * what () const throw () {
    return "NotAllowedError";
  };
};

#endif
