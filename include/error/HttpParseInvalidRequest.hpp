#ifndef HTTPPARSEINVALIDREQUEST_HPP
#define HTTPPARSEINVALIDREQUEST_HPP

#include <exception>

class HttpParseInvalidRequest : public std::exception
{
  const char * what () const throw () {
    return "HttpParseInvalidRequest";
  };
};

#endif
