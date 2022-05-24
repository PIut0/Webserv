#ifndef RESPONSEHEADER_HPP
#define RESPONSEHEADER_HPP

#include "Header.hpp"
#include "HTTPResponseHeader.hpp"

class HTTPResponseHeader;

class ResponseHeader : public Header
{
 public:
  ResponseHeader();
  ~ResponseHeader();

  void SetUpHeader(HttpContentsType &type);
  void SetUpHeaderResponse(HttpContentsType &type);

//  protected:
  HTTPResponseHeader  *response_;
};

#endif
