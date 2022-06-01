#ifndef HTTPPARSEINVALIDBODY_HPP
#define HTTPPARSEINVALIDBODY_HPP

#include <exception>

class HttpParseInvalidBody : public std::exception
{
	const char * what () const throw () {
    return "HttpParseInvalidBody";
  };
};

#endif
