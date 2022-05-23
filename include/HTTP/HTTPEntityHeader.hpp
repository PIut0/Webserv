#ifndef HTTPENTITYHEADER_HPP
#define HTTPENTITYHEADER_HPP

#include <string>

class HTTPEntityHeader
{
 private:
  std::string content_length_;
  std::string content_language_;
  std::string content_type_;
  std::string content_encoding_;

 public:
  HTTPEntityHeader();
  ~HTTPEntityHeader();
};

#endif
