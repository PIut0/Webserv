#ifndef REQUESTHEADER_HPP
#define REQUESTHEADER_HPP

#include "Header.hpp"

class RequestHeader : public Header
{
 public:
  RequestHeader();
  RequestHeader(const std::string &data);
  RequestHeader(const RequestHeader &origin);
  ~RequestHeader();

  RequestHeader& operator=(const RequestHeader &rv);

  void            SetMethod(const int &method);
  void            SetHost(const std::string &host);
  void            SetHttpVersion(const int &major, const int &minor);
  void            SetRequest(const std::string &request);
  void            SetItem(const std::string &line);
  void            SetItem(const std::string &key, const std::string &value);
  void            SetBody(const std::string &body);

  wsv_header_t&   GetItem(const std::string &key);
  req_header_it_t FindItem(const std::string &key);

  void  Print();
  void  PrintRequestLine();
  void  PrintHeaderLine();
  void  PrintBodyLine();

  std::string ToString();
  std::string MethodToString();
  std::string HttpVersionToString();

  // request line
  int           method;
  std::string   host;
  short         http_major;
  short         http_minor;
  // header line
  req_header_t  conf;
  // body line
  std::string   body; // TODO body를 복사하는게 맞는가

 private:
  int   ParseHeaderLine(const std::string &data);
  int   ParseRequestLine(const std::string &data);
  int   ParseBodyLine(const std::string &data);

  size_t        pos_;
};


#endif

