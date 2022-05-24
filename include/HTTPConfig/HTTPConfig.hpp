#ifndef HTTPCONFIG_HPP
#define HTTPCONFIG_HPP

#include "System.hpp"


#include <map>
#include <climits>
#include "utils.hpp"


typedef u_char HttpContentsType;

/* HTTP Header Group Start with H_*
   each type can be used in combination with header type. */
#define G_GENERAL   0
#define G_ENTITY    1
#define G_RESPONSE  2
#define G_REQUEST   3


/* HTTP Header Type Start with C_*
   Each type can be used in combination with header type.
   This list is based on https://velog.io/@cham/HTTP-Header에는-어떤-정보들이-담겨있을까 */
// General
#define H_DATE                    0
#define H_PRAGMA                  1
#define H_CACHE_CONTROL           2
#define H_VIA                     3
#define H_CONNECTION              4
#define H_TRANSFER_ENCODING       5
// Entity
#define H_CONTENT_TYPE            0
#define H_CONTEHT_LENGTH          1
#define H_CONTENT_LANGUAGE        2
#define H_CONTENT_LOCATION        3
#define H_CONTENT_DISPOSITION     4
#define H_CONTENT_SECURITY_POLICY 5
#define H_CONTENT_ENCODING        6
#define H_LOCATION                8
#define H_LAST_MODIFIED           9
#define H_ALLOW                   10
#define H_EXPIRES                 11
#define H_ETAG                    12
// Request
#define H_HOST                    1
#define H_USER_AGENT              2
#define H_REFERER                 3
#define H_ACCEPT                  4
#define H_ACCEPT_CHARSET          5
#define H_ACCEPT_LANGUAGE         6
#define H_ACCEPT_ENCODING         7
#define H_IF_MODIFIED_SINCE       8
#define H_AUTHORIZATION           9
#define H_ORIGIN                  10
#define H_COOKIE                  11
// Response
#define H_SERVER                  0
#define H_AGE                     1
#define H_REFERRER_POLICY         2
#define H_WWW_AUTHENTICATE        3
#define H_PROXY_AUTHENTICATE      4
#define H_SET_COOKIE              5


/* If the number of commands exceeds the range that can be expressed,
   adjust the G_RANGE. */
#define G_RANGE     4
#define G_MASK      (UCHAR_MAX >> (CHAR_BIT - G_RANGE))
#define H_MASK      (UCHAR_MAX >> G_RANGE)

#define SET_CONTENTS(G, H)  (G << G_RANGE | H)

#define G_MASKING(A)  A & G_MASK
#define H_MASKING(A)  A & H_MASK


class HTTPConfig : public System
{
 public:
  HttpContentsType FindType(std::string &command);

 private:
  HTTPConfig();
  ~HTTPConfig();

  std::map<std::string, HttpContentsType> data;
};

#endif
