#ifndef RESPONSEHEADER_HPP
#define RESPONSEHEADER_HPP

#include <string>

#include "HTTPGeneralHeader.hpp"
#include "HTTPEntityHeader.hpp"
#include "HTTPResponseHeader.hpp"

class ResponseHeader
{
 public:
  ResponseHeader();
  ~ResponseHeader();

 private:
  HTTPEntityHeader    &http_entitiy_header_;
  HTTPGeneralHeader   &http_general_header_;
  HTTPResponseHeader  &http_Response_header_;
};

#endif
