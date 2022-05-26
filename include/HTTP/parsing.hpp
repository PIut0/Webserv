#ifndef PARSING_HPP
#define PARSING_HPP

#include "RequestHeader.hpp"
#include "Header.hpp"
#include "utils.hpp"

int HttpParseRequestLine(RequestHeader &req_h, char *value);
int HttpParseHeaderLine(RequestHeader &req_h);

#endif
