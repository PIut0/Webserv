#ifndef HTTPPARSEINVALIDRESPONSE_HPP
#define HTTPPARSEINVALIDRESPONSE_HPP

#include <exception>

class HttpParseInvalidResponse : public std::exception
{
  const char * what () const throw () {
    return "HttpParseInvalidResponse";
  };
};

#endif
