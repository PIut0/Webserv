#ifndef NOTFOUND_HPP
#define NOTFOUND_HPP

#include <exception>

class NotFoundError : public std::exception
{
 public:
	const char * what () const throw () {
    return "NotFound";
  };
};

#endif
