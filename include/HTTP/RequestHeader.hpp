#ifndef REQUESTHEADER_HPP
#define REQUESTHEADER_HPP

#include "Header.hpp"
#include "AlreadyExistKey.hpp"


class RequestHeader : public Header
{
 public:
  RequestHeader();
  RequestHeader(std::string &data);
  ~RequestHeader();

  void            SetItem(std::string &key, std::string &value);
  wsv_header_t&   GetItem(std::string &key);
  req_header_it_t FindItem(std::string &key);

  void            PrintItem();
  void            Print();

  req_header_t  conf;
  std::string   &buf;

  int           method;
  std::string   host;
  short         http_major;
  short         http_minor;
  size_t        pos;
};


#endif

  // wsv_header_t      *host;
  // wsv_header_t      *connection;
  // wsv_header_t      *if_modified_since;
  // wsv_header_t      *if_unmodified_since;
  // wsv_header_t      *if_match;
  // wsv_header_t      *if_range;

  // wsv_header_t      *transfer_encoding;
  // wsv_header_t      *te;
  // wsv_header_t      *expect;
  // wsv_header_t      *upgrade;

  // wsv_header_t      *if_none_match;
  // wsv_header_t      *user_agent;
  // wsv_header_t      *referer;
  // wsv_header_t      *content_length;
  // wsv_header_t      *content_range;
  // wsv_header_t      *content_type;

  // wsv_header_t      *range;

  // wsv_header_t      *accept_encoding;
  // wsv_header_t      *via;

  // wsv_header_t      *authorization;

  // wsv_header_t      *keep_alive;

  // wsv_header_t      *x_real_ip;

  // wsv_header_t      *accept;
  // wsv_header_t      *accept_language;

  // wsv_header_t      *depth;
  // wsv_header_t      *destination;
  // wsv_header_t      *overwrite;
  // wsv_header_t      *date;



