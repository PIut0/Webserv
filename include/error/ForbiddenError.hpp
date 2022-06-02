#ifndef FORBIDDEN_HPP
#define FORBIDDEN_HPP

#include <exception>

class ForbiddenError : public std::exception
{
 public:
	const char * what () const throw () {
    return "Forbidden";
  };
};

#endif
