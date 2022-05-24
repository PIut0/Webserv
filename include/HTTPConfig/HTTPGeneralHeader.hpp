#ifndef HTTPGENERALHEADER_HPP
#define HTTPGENERALHEADER_HPP

#include <string>

// TODO General 생성자 구현하기
class HTTPGeneralHeader
{
 public:
  HTTPGeneralHeader();
  ~HTTPGeneralHeader();

//  private:
  std::string date_;
  std::string connection_; // Keep-alive or close
  std::string cache_control_;
};

#endif
