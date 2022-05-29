#ifndef REQUESTHEADER_HPP
#define REQUESTHEADER_HPP

#include "Header.hpp"
#include "AlreadyExistKey.hpp"


class RequestHeader : public Header
{
 public:
  RequestHeader(std::string &data);
  RequestHeader(const RequestHeader &origin);
  ~RequestHeader();

  RequestHeader& operator=(const RequestHeader &rv);

  void            SetItem(std::string &key, std::string &value);
  wsv_header_t&   GetItem(std::string &key);
  req_header_it_t FindItem(std::string &key);

  int   HttpParseHeaderLine();
  int   HttpParseRequestLine();

  void  PrintRequestLine();
  void  PrintHeaderLine();
  void  Print();

  std::string   *buf;
  std::string   *body;

  req_header_t  conf;
  int           method;
  std::string   host;
  short         http_major;
  short         http_minor;
  size_t        pos;

 private:
  RequestHeader();
};


#endif

