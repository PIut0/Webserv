#ifndef REQUESTHEADER_HPP
#define REQUESTHEADER_HPP

#include "Header.hpp"
#include "HTTPRequestHeader.hpp"

class HTTPRequestHeader;

class RequestHeader : public Header
{
 public:
  RequestHeader();
  ~RequestHeader();

  void SetUpHeader(HttpContentsType &type);
  void SetUpHeaderRequest(HttpContentsType &type);

//  protected:
  HTTPRequestHeader *request_;
};

#endif
