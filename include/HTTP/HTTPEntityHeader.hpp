#ifndef HTTPENTITYHEADER_HPP
#define HTTPENTITYHEADER_HPP

#include <string>

class HTTPEntityHeader
{
 public:
  HTTPEntityHeader(std::vector<std::string> &data);


 private:
  HTTPEntityHeader();
  ~HTTPEntityHeader();

  void Init(std::string &data);

  std::string content_length_;
  std::string content_language_;
  std::string content_type_;
  std::string content_encoding_;
};

#endif
