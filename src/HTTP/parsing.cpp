#include "parsing.hpp"

int HttpParseRequestLine(RequestHeader &req_h)
{
  size_t  pos = 0, host_start, host_end;
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

  while (state != wsb_done)
  {
    ch = (*req_h.buf)[pos];

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
          if (pos == 3 && wsb_str_3cmp((*req_h.buf), 'G', 'E', 'T')) {
            req_h.method = HTTP_GET;
          }

          else if (pos == 3 && wsb_str_3cmp((*req_h.buf), 'P', 'U', 'T')) {
            req_h.method = HTTP_POST;
          }

          else if (pos == 4 && wsb_str_4cmp((*req_h.buf), 'P', 'O', 'S', 'T')) {
            req_h.method = HTTP_PUT;
          }

          else if (pos == 6 && wsb_str_6cmp((*req_h.buf), 'D', 'E', 'L', 'E', 'T', 'E')) {
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
          host_start = pos;
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
            host_end = pos;
            req_h.host = (*req_h.buf).substr(host_start, host_end - host_start + 1);
            state = wsb_before_option;
            break;

          default:
            break;
        }

        break;

      case wsb_before_option:
        if (wsv_str_cmp((*req_h.buf), pos, 'H', 'T', 'T', 'P', '/') && \
                         (*req_h.buf)[pos + 5] >= '0' && (*req_h.buf)[pos + 5] <= '9' && \
                         (*req_h.buf)[pos + 6] == '.' && \
                         (*req_h.buf)[pos + 7] >= '0' && (*req_h.buf)[pos + 7] <= '9' ) {
          req_h.http_major = (*req_h.buf)[5] - '0';
          req_h.http_minor = (*req_h.buf)[7] - '0';
          pos += 7; // 하드코딩..
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
    ++pos;
  }
  req_h.pos = pos;
  return WSV_OK;
}

int HttpParseHeaderLine(RequestHeader &req_h)
{
  // char          *key_start, *key_end, *value_start, *value_end;
  size_t        key_start, key_end, value_start, value_end;
  std::string   key, value;
  u_char        ch;
  size_t        pos;

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

  for (pos = req_h.pos ; (*req_h.buf)[pos] ; ++pos) {
    ch = (*req_h.buf)[pos];

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
          key_start = pos;
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
            key_end = pos - 1;
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
          value_start = pos;
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
            value_end = pos - 1;
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
            key = (*req_h.buf).substr(key_start, key_end - key_start + 1);
            value = (*req_h.buf).substr(value_start, value_end - value_start + 1);
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
            key_start = pos;
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
