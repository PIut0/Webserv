#ifndef RESPONSEHEADER_HPP
#define RESPONSEHEADER_HPP

#include "Header.hpp"

class HTTPResponseHeader;

class ResponseHeader : public Header
{
 public:
  ResponseHeader();
  ResponseHeader(const ResponseHeader &origin);
  ~ResponseHeader();

  ResponseHeader& operator=(const ResponseHeader &rv);

  void            SetBody(const std::string &body);
  void            SetItem(const std::string &line);
  void            SetItem(const std::string &key, const std::string &value);

  wsv_header_t&   GetItem(const std::string &key);
  res_header_it_t FindItem(const std::string &key);

  void  Parse(const std::string &data);

  std::string  ToString();

  void  Clear();

  void  Print();
  void  PrintResponseLine();
  void  PrintHeaderLine();
  void  PrintBody();

  std::string   status_code;
  std::string   status_msg;
  res_header_t  conf;
  std::string   body;

 private:
  int   ParseHeaderLine(const std::string &data);
  int   ParseBody(const std::string &data);

  size_t             pos_;
};


#endif
