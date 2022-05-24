#ifndef HTTPREQUESTHEADER_HPP
#define HTTPREQUESTHEADER_HPP

#include <string>

// TODO Request 생성자 구현하기
class HTTPRequestHeader
{
 public:
  HTTPRequestHeader() {};
  ~HTTPRequestHeader() {};

//  private:
  std::string host_;
  std::string user_agent_;
  std::string accept_;
  std::string authorization_;
  std::string origin_;
  std::string referer_;
};

#endif
