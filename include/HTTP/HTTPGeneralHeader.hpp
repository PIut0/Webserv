#ifndef HTTPGENERALHEADER_HPP
#define HTTPGENERALHEADER_HPP

#include <string>

class HTTPGeneralHeader
{
 private:
  std::string date_;
  std::string connection_; // Keep-alive or close
  std::string cache_control_;

 public:
  HTTPGeneralHeader();
  ~HTTPGeneralHeader();
};

#endif
