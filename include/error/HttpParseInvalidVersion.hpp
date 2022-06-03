#ifndef HTTPPARSEINVALIDVERSION_HPP
#define HTTPPARSEINVALIDVERSION_HPP

#include <exception>

class HttpParseInvalidVersion : public std::exception
{
  const char * what () const throw () {
    return "HttpParseInvalidVersion";
  };
};

#endif
