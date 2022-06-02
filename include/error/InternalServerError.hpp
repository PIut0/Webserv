#ifndef INTERNALSERVER_HPP
#define INTERNALSERVER_HPP

#include <exception>

class InternalServerError : public std::exception
{
 public:
	const char * what () const throw () {
    return "InternalServer";
  };
};

#endif
