#ifndef NOTIMPLEMENTEDERROR_HPP
#define NOTIMPLEMENTEDERROR_HPP

#include <exception>

class NotImplementedError : public std::exception
{
	const char * what () const throw () {
    return "NotImplementedError";
  };
};

#endif
