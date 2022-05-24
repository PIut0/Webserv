#ifndef HTTPRESPONSEHEADER_HPP
#define HTTPRESPONSEHEADER_HPP

#include <string>

// TODO Response 생성자 구현하기
class HTTPResponseHeader
{
 public:
  HTTPResponseHeader() {};
  ~HTTPResponseHeader() {};

//  private:
  std::string access_control_allow_origin_;
  std::string allow_;
  std::string content_disposition_;
  std::string location_;
  std::string content_security_policy_;
};

#endif
