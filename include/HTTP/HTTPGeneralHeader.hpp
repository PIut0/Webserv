#ifndef HTTPGENERALHEADER_HPP
#define HTTPGENERALHEADER_HPP

#include <string>

class HTTPGeneralHeader
{
 public:
  HTTPGeneralHeader();
  ~HTTPGeneralHeader();

 private:
  std::string date_;
  std::string connection_; // Keep-alive or close
  std::string cache_control_;
};

#endif
