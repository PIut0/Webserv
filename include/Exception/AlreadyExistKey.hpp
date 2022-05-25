#ifndef ALREADYEXISTKEY_HPP
#define ALREADYEXISTKEY_HPP

#include <exception>

class AlreadyExistKey : public std::exception
{
	const char * what () const throw () {
    return "hi";
  };
};

#endif
