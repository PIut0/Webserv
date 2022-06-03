#ifndef HttpParseInvalidVersion
#define HttpParseInvalidVersion

#include <exception>

class HttpParseInvalidVersion : public std::exception
{
  const char * what () const throw () {
    return "HttpParseInvalidVersion";
  }
}

#endif
