#ifndef HTTPENTITYHEADER_HPP
#define HTTPENTITYHEADER_HPP

#include <string>

// TODO Entity 생성자 구현하기
class HTTPEntityHeader
{
 public:
  HTTPEntityHeader() {};
  ~HTTPEntityHeader() {};

// TODO 유연한 개발을 위해 각 변수들을 미리 만들어두는게 아니라 아래와 같이 활용하기
// ex)
// std::map<H_TYPE, std::string>;
// map[H_TYPE] = "content";

//  private:
  std::string content_length_;
  std::string content_language_;
  std::string content_type_;
  std::string content_encoding_;
};

#endif
