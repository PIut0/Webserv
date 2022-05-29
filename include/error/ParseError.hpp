#ifndef PARSEERROR_HPP
#define PARSEERROR_HPP

#include <exception>
#include <iostream>

#define REQUEST_LINE  0
#define HEADER_LINE   1

class ParseError : public std::exception
{
	const char * what () const throw () {
    return "Parse ERROR";
  };
};

#endif
