#ifndef REQUESTHEADER_HPP
#define REQUESTHEADER_HPP

#include "Header.hpp"

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

  void  Parse();

  void  Print();
  void  PrintRequestLine();
  void  PrintHeaderLine();

  // before parse
  std::string   *buf;

  // request line
  int           method;
  std::string   host;
  short         http_major;
  short         http_minor;
  size_t        pos;
  // header line
  req_header_t  conf;
  // body line
  std::string   *body;

 private:
  RequestHeader();

  int   ParseHeaderLine();
  int   ParseRequestLine();
};


#endif

