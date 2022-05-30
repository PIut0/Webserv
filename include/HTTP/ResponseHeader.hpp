#ifndef RESPONSEHEADER_HPP
#define RESPONSEHEADER_HPP

#include "Header.hpp"

class HTTPResponseHeader;

class ResponseHeader : public Header
{
 public:
  ResponseHeader(const ResponseHeader &origin);
  ~ResponseHeader();

  ResponseHeader& operator=(const ResponseHeader &rv);

  void            SetItem(std::string &key, std::string &value);
  wsv_header_t&   GetItem(std::string &key);
  res_header_it_t FindItem(std::string &key);

  void  Parse();

  void  Print();
  void  PrintRequestLine();
  void  PrintHeaderLine();


  // before parse
  std::string   *buf;

  // header line
  res_header_t  conf;
  // header line
  // body line
  std::string   *body;

 private:
  ResponseHeader();

  int ParseHeaderLine();
  int ParseRequestLine();
};


#endif
