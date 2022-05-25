#include "parsing.hpp"

int HttpParseRequestLine(RequestHeader &req_h, char *value)
{
  char  *ptr = value, *m;
  // char  *pos = ptr;
  // char  *bpos = ptr;
  u_char ch;

  enum {
    wsb_start = 0,
    wsb_method,
    wsb_before_uri,
    wsb_in_uri,
    wsb_before_option,
    wsb_in_option,
    wsb_almost_done,
    wsb_done
  } state;

  state = wsb_start;

  // parse_request_header
  while (state != wsb_done)
  {
    ch = *ptr;
    std::cout << "[" << state << "]" << "ch : " << ch << std::endl;
    switch (state)
    {
      case wsb_start:
        if ((ch < 'A' || ch > 'Z') && ch != '_' && ch != '-') {
          return WSV_HTTP_PARSE_INVALID_METHOD;
        }

        state = wsb_method;
        break;

      case wsb_method:
        if (ch == ' ') {
          m = value;
          if (ptr - m == 3 && wsb_str_3cmp(m, 'G', 'E', 'T')) {
            req_h.method = HTTP_GET;
          }

          else if (ptr - m == 3 && wsb_str_3cmp(m, 'P', 'U', 'T')) {
            req_h.method = HTTP_POST;
          }

          else if (ptr - m == 4 && wsb_str_4cmp(m, 'P', 'O', 'S', 'T')) {
            req_h.method = HTTP_PUT;
          }

          else if (ptr - m == 6 && wsb_str_6cmp(m, 'D', 'E', 'L', 'E', 'T', 'E')) {
            req_h.method = HTTP_DELETE;
          }

          else {
            return WSV_HTTP_PARSE_INVALID_METHOD;
          }

          state = wsb_before_uri;

        }

        break;

      case wsb_before_uri:
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '/' ) {
          req_h.host_start = ptr;
          state = wsb_in_uri;
          break;
        }

        switch (ch) {
          case ' ':
            break;

          default:
            return WSV_HTTP_PARSE_INVALID_REQUEST;
        }

        break;

      case wsb_in_uri:
        switch (ch) {
          case ' ':
            req_h.host_end = ptr;
            state = wsb_before_option;
            break;
          default:
            break;
        }

        break;

      case wsb_before_option:
        if (wsb_str_5cmp(ptr, 'H', 'T', 'T', 'P', '/') && \
                         ptr[5] >= '0' && ptr[5] <= '9' && \
                         ptr[6] == '.' && \
                         ptr[7] >= '0' && ptr[7] <= '9' ) {
          req_h.http_major = ptr[5] - '0';
          req_h.http_minor = ptr[7] - '0';
          ptr += 7; // 하드코딩..
          break;
        }

        switch (ch) {
          case ' ':
            break;

          case CR:
            state = wsb_almost_done;
            break;

          default:
            return WSV_HTTP_PARSE_INVALID_REQUEST;
        }

        break;

      case wsb_almost_done:
        switch (ch) {
          case LF:
            state = wsb_done;
            break;

          default:
            return WSV_HTTP_PARSE_INVALID_REQUEST;
        }

        break;

      default:
        return WSV_HTTP_PARSE_INVALID_REQUEST;

    }
    ++ptr;
  }
  req_h.pos = ptr;
  return WSV_OK;
}

int HttpParseHeaderLine(RequestHeader &req_h)
{
  // HOST 부터 파싱 시작
  std::cout << req_h.pos << std::endl;
  return WSV_OK;
}
