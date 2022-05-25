#ifndef RESPONSEHEADER_HPP
#define RESPONSEHEADER_HPP

#include "RequestHeader.hpp"
#include "utils.hpp"

int HttpParseRequestLine(RequestHeader &req_h, char *value);
int HttpParseHeaderLine(RequestHeader &req_h);

#endif
