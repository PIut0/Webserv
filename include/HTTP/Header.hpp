#ifndef HEADER_HPP
#define HEADER_HPP

#include <string>

#include "HTTPConfig.hpp"
#include "HTTPGeneralHeader.hpp"
#include "HTTPEntityHeader.hpp"

class HTTPEntityHeader;
class HTTPGeneralHeader;

class Header
{
 public:
  Header();
  virtual ~Header();

  void SetUpHeaderGeneral(HttpContentsType &type);
  void SetUpHeaderEntity(HttpContentsType &type);
  virtual void SetUpHeader(HttpContentsType &type) = 0;

//  protected:
  HTTPEntityHeader  *entitiy_;
  HTTPGeneralHeader *general_;
};

#endif
