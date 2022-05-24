#ifndef REQUESTHEADER_HPP
#define REQUESTHEADER_HPP

#include <string>

#include "HTTPGeneralHeader.hpp"
#include "HTTPEntityHeader.hpp"
#include "HTTPRequestHeader.hpp"

class RequestHeader
{
 public:
  RequestHeader();
  ~RequestHeader();

 private:
  HTTPEntityHeader  &http_entitiy_header_;
  HTTPGeneralHeader &http_general_header_;
  HTTPRequestHeader &http_request_header_;
};

#endif
