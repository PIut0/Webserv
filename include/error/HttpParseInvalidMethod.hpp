#ifndef HTTPPARSEINVALIDMETHOD_HPP
#define HTTPPARSEINVALIDMETHOD_HPP

#include <exception>

class HttpParseInvalidMethod : public std::exception
{
	const char * what () const throw () {
    return "HttpParseInvalidMethod";
  };
};

#endif
