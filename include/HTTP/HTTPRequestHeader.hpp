#ifndef HTTPREQUESTHEADER_HPP
#define HTTPREQUESTHEADER_HPP

#include <string>

class HTTPRequestHeader
{
 private:
  std::string host_;
  std::string user_agent_;
  std::string accept_;
  std::string authorization_;
  std::string origin_;
  std::string referer_;

 public:
  HTTPRequestHeader();
  ~HTTPRequestHeader();
};

#endif
