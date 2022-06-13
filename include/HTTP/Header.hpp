#ifndef HEADER_HPP
#define HEADER_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "System.hpp"
#include "Color.hpp"
#include "AlreadyExistKey.hpp"
#include "Defines.hpp"
#include "utils.hpp"
#include "HttpStatusCode.hpp"

#define  HTTP_UNKNOWN  0x00000001
#define  HTTP_GET      0x00000002
#define  HTTP_POST     0x00000004
#define  HTTP_PUT      0x00000008
#define  HTTP_DELETE   0x00000010

#define  CRLF   "\r\n"
#define  CR     '\r'
#define  LF     '\n'

#define  WSV_OK        0
#define  WSV_ERROR    -1
#define  WSV_ABORT    -2

#define  WSV_HTTP_PARSE_INVALID_METHOD      10
#define  WSV_HTTP_PARSE_INVALID_REQUEST     11
#define  WSV_HTTP_PARSE_INVALID_VERSION     12

#define wsb_str_2cmp(p, c0, c1) \
  p[0] == c0 && p[1] == c1
#define wsb_str_3cmp(p, c0, c1, c2) \
  p[0] == c0 && p[1] == c1 && p[2] == c2
#define wsb_str_4cmp(p, c0, c1, c2, c3) \
  p[0] == c0 && p[1] == c1 && p[2] == c2 && p[3] == c3
#define wsb_str_5cmp(p, c0, c1, c2, c3, c4) \
  p[0] == c0 && p[1] == c1 && p[2] == c2 && p[3] == c3 && p[4] == c4
#define wsb_str_6cmp(p, c0, c1, c2, c3, c4, c5) \
  p[0] == c0 && p[1] == c1 && p[2] == c2 && p[3] == c3 && p[4] == c4 && p[5] == c5
#define wsb_str_7cmp(p, c0, c1, c2, c3, c4, c5, c6) \
  p[0] == c0 && p[1] == c1 && p[2] == c2 && p[3] == c3 && p[4] == c4 && p[5] == c5 && p[6] == c6
#define wsb_str_8cmp(p, c0, c1, c2, c3, c4, c5, c6, c7) \
  p[0] == c0 && p[1] == c1 && p[2] == c2 && p[3] == c3 && p[4] == c4 && p[5] == c5 && p[6] == c6 && p[7] == c7
#define wsv_str_2cmp_p(s, p, c0, c1) \
  s[p] == c0 && s[p + 1] == c2
#define wsv_str_5cmp_p(s, p, c0, c1, c2, c3, c4) \
  s[p] == c0 && s[p + 1] == c1 && s[p + 2] == c2  && s[p + 3] == c3  && s[p + 4] == c4
#define wsv_str_4cmp_p(s, p, c0, c1, c2, c3) \
  s[p] == c0 && s[p + 1] == c1 && s[p + 2] == c2  && s[p + 3] == c3


typedef struct {
  std::string key;
  std::string value;
} wsv_header_t;

typedef std::map<std::string, wsv_header_t *> req_header_t;
typedef std::map<std::string, wsv_header_t *>::iterator req_header_it_t;

typedef std::vector<wsv_header_t *> res_header_t;
typedef std::vector<wsv_header_t *>::iterator res_header_it_t;

class Header : public System
{
 public:
  Header();
  virtual ~Header();

  void CheckValidateKey(std::string &key);
  void CheckValidateValue(std::string &value);

  virtual void SetItem(const std::string &key, const std::string &value) = 0;
  virtual wsv_header_t& GetItem(const std::string &key) = 0;

  int state;
};

#endif
