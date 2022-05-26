#include "parsing.hpp"

int HttpParseRequestLine(RequestHeader &req_h, char *value)
{
  char  *ptr = value, *m;
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
  char          *ptr, *key_start, *key_end, *value_start, *value_end;
  std::string   key, value;
  u_char        ch;



  enum {
    wsv_start = 0,
    wsv_header_key, // : 나오기 전까지
    wsv_header_value_before, // 문자열 나오기 전까지
    wsv_header_value, // CR나오기 전까지
    wsv_invalid_key, // CR 나오기 전까지
    wsv_invalid_key_newline,
    wsv_valide_key_newline,
    wsv_almost_done,
    wsv_done // LF 나오면 끝
  } state;

  state = wsv_start;

  for (ptr = req_h.pos ; *ptr ; ++ptr) {
    ch = *ptr;

    switch (state)
    {
      case wsv_start:
        if ((ch < 'A' || ch > 'Z') && ch != '_' && ch != '-') {
          state = wsv_invalid_key;
        }

        if (ch == ' ') {
          break;
        }
        // TODO 대문자 시작 체크 해야하나 ??
        if (isalpha(ch)) {
          key_start = ptr;
          state = wsv_header_key;
        }
        break;

      case wsv_header_key:
      // :나오면 wsv_header_value_before로
      // CR나오면 wsv_invalid_key_newline으로
      // 아니면 break;
        switch (ch)
        {
          case ':':
            key_end = ptr - 1;
            state = wsv_header_value_before;
            break;

          case CR:
            state = wsv_invalid_key_newline;
            break;

          default:
            break;
        }

        break;

      case wsv_header_value_before:
      // 문자열 나오면 ptr저장 wsv_header_value로
      // 공백 나오면 break;
        if (ch != ' ' && isprint(ch)) {
          state = wsv_header_value;
          value_start = ptr;
          break;
        }

        switch (ch)
        {
          case ' ':
            break;

          case CR:
            state = wsv_invalid_key_newline;
            break;

          default:
            break;
        }

        break;

      case wsv_header_value:
      // CR 나오면 wsv_newline으로
      // 아니면 break;
        switch (ch)
        {
          case CR:
            value_end = ptr - 1;
            state = wsv_valide_key_newline;
            break;

          default:
            break;
        }

        break;

      case wsv_invalid_key:
      // CR 나오면 wsv_invalid_key_newline으로
      // 아니면 break;
        switch (ch)
        {
          case CR:
            state = wsv_invalid_key_newline;
            break;

          default:
            break;
        }
        break;
      case wsv_invalid_key_newline:
      // LF 나오면 wsv_almost_done으로
      // 아니면 error
        switch (ch)
        {
        case LF:
          state = wsv_almost_done;
          break;

        default:
          break;
        }

        break;

      case wsv_valide_key_newline:
      // LF 나오면 wsv_almost_done으로
      // 아니면 error
        switch (ch)
        {
          case LF:
            state = wsv_almost_done;
            key = strndup(key_start, key_end - key_start + 1);
            value = strndup(value_start, value_end - value_start + 1);
            req_h.SetItem(key, value);
            break;

          default:
            break;
        }

        break;

      case wsv_almost_done:
      // CR 나오면 almost_done
      // 아니면 wsv_start로
        switch (ch)
        {
          case CR:
            state = wsv_done;
            break;

          default:
            key_start = ptr;
            state = wsv_header_key;
            break;
        }

        break;

      case wsv_done:
      // LF 나오면 끝
      // 아니면 에러
        switch (ch)
        {
          case LF:
            return WSV_OK;

          default:
            break;
        }

        break;

      default:
        break;
    }
  }

  return WSV_HTTP_PARSE_INVALID_REQUEST;
}
