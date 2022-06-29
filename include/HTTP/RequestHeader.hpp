#ifndef REQUESTHEADER_HPP
#define REQUESTHEADER_HPP

#include "Header.hpp"

#define CGI_PHP 0
#define CGI_BLA 1
#define CGI_HEADER_SIZE 2

class RequestHeader : public Header
{
 public:
  RequestHeader();
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
  int             SetChunked(const std::string &chunked);

  wsv_header_t&   GetItem(const std::string &key);
  req_header_it_t FindItem(const std::string &key);

  char** ToCgi(const std::string &path);

  void  Parse(const std::string &data);

  std::string ToString();
  std::string MethodToString();
  std::string HttpVersionToString();

  void  Clear();

  void  Print();
  void  PrintRequestLine();
  void  PrintHeaderLine();
  void  PrintBodyLine();


  int           method;
  std::string   host;
  short         http_major;
  short         http_minor;
  req_header_t  conf;
  std::string   body;

 private:
  int   ParseHeaderLine(const std::string &data);
  int   ParseRequestLine(const std::string &data);
  int   ParseBodyLine(const std::string &data);

  size_t        pos_;
};


#endif

