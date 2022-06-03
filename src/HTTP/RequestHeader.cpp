#include "RequestHeader.hpp"

RequestHeader::RequestHeader() : pos_(0) {}

RequestHeader::RequestHeader(const RequestHeader &origin)
{
  *this = origin;
}

RequestHeader::~RequestHeader() {}

RequestHeader& RequestHeader::operator=(const RequestHeader &rv)
{
  this->conf = rv.conf;
  this->method = rv.method;
  this->host = rv.host;
  this->http_major = rv.http_major;
  this->http_minor = rv.http_minor;
  this->pos_ = rv.pos_;

  return *this;
}

void RequestHeader::SetMethod(const int &method)
{
  this->method = method;
}

void RequestHeader::SetHost(const std::string &host)
{
  this->host = host;
}

void RequestHeader::SetHttpVersion(const int &major, const int &minor)
{
  this->http_major = major;
  this->http_minor = minor;
}


void RequestHeader::SetRequest(const std::string &request)
{
  ParseRequestLine(request + CRLF + CRLF);
}

void RequestHeader::SetItem(const std::string &line)
{
  size_t index = line.find(":");
  std::string key = line.substr(0, index);
  std::string value = line.substr(index + 2, line.length() - 1);
  // std::string trim_value = trim(value);

  SetItem(key, value);
}

void RequestHeader::SetItem(const std::string &key, const std::string &value)
{
  if (FindItem(key) != this->conf.end())
    throw AlreadyExistKey();

  wsv_header_t *el = new wsv_header_t();
  el->key = key;
  el->value = value;
  this->conf[key] = el;
}

void RequestHeader::SetBody(const std::string &body)
{
  this->body = body;
}

int RequestHeader::SetChunked(const std::string &chunked)
{
  std::string ret;
  int contents_length;
  int pos;

  if (chunked.length() == 0) {
    return 0;
  }

  pos = chunked.find(CRLF);
  contents_length = atoi(chunked.substr(0, pos).c_str());
  if (contents_length == 0) return 0;

  if (wsv_str_4cmp_p(chunked, chunked.length() - 4, '\r', '\n', '\r', '\n')) {
    this->body += chunked.substr(pos + 2, chunked.length() - (pos + 2) - 4);
    return contents_length;
  }

  return -1;
}

req_header_it_t RequestHeader::FindItem(const std::string &key)
{
  return this->conf.find(key);
}

wsv_header_t& RequestHeader::GetItem(const std::string &key)
{
  req_header_it_t it = FindItem(key);
  if (it == this->conf.end()) {
    this->SetItem(key, "");
    return GetItem(key);
  }
  return *(it->second);
}
void  RequestHeader::Parse(const std::string &data)
{
  ParseRequestLine(data);
  ParseHeaderLine(data);
  ParseBodyLine(data);
}
// TODO return 에서 throw ParseError()로 바꾸기
int RequestHeader::ParseRequestLine(const std::string &data)
{
  size_t  pos, host_start, host_end;
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
  pos = this->pos_;

  while (state != wsb_done)
  {
    ch = data[pos];

    switch (state)
    {
      case wsb_start:
        if ((ch < 'A' || ch > 'Z') && ch != '_' && ch != '-') {
          throw HttpParseInvalidMethod();
        }

        state = wsb_method;
        break;

      case wsb_method:
        if (ch == ' ') {
          if (pos == 3 && wsb_str_3cmp(data, 'G', 'E', 'T')) {
            this->method = HTTP_GET;
          }

          else if (pos == 3 && wsb_str_3cmp(data, 'P', 'U', 'T')) {
            this->method = HTTP_POST;
          }

          else if (pos == 4 && wsb_str_4cmp(data, 'P', 'O', 'S', 'T')) {
            this->method = HTTP_PUT;
          }

          else if (pos == 6 && wsb_str_6cmp(data, 'D', 'E', 'L', 'E', 'T', 'E')) {
            this->method = HTTP_DELETE;
          }

          else {
            throw HttpParseInvalidMethod();
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
            throw HttpParseInvalidRequest();
        }

        break;

      case wsb_in_uri:
        switch (ch) {
          case ' ':
            host_end = pos;
            this->host = data.substr(host_start, host_end - host_start);
            state = wsb_before_option;
            break;

          default:
            break;
        }

        break;

      case wsb_before_option:
        if (wsv_str_5cmp_p(data, pos, 'H', 'T', 'T', 'P', '/') && \
                         data[pos + 5] >= '0' && data[pos + 5] <= '9' && \
                         data[pos + 6] == '.' && \
                         data[pos + 7] >= '0' && data[pos + 7] <= '9' ) {
          this->http_major = data[pos + 5] - '0';
          this->http_minor = data[pos + 7] - '0';
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
            throw HttpParseInvalidVersion();
        }

        break;

      case wsb_almost_done:
        switch (ch) {
          case LF:
            state = wsb_done;
            this->pos_ = pos + 1;
            break;

          default:
            throw HttpParseInvalidRequest();
        }

        break;

      default:
        throw HttpParseInvalidRequest();

    }
    ++pos;
  }
  this->pos_ = pos;
  return WSV_OK;
}

int RequestHeader::ParseHeaderLine(const std::string &data)
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

  for (pos = this->pos_ ; data[pos] ; ++pos) {
    ch = data[pos];

    switch (state)
    {
      case wsv_start:
        if ((ch < 'A' || ch > 'Z') && ch != '_' && ch != '-') {
          state = wsv_invalid_key;
        }

        if (ch == CR) {
          state = wsv_done;
          break;
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
            key = data.substr(key_start, key_end - key_start + 1);
            value = data.substr(value_start, value_end - value_start + 1);
            this->SetItem(key, value);
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
            this->pos_ = pos + 1;
            return WSV_OK;

          default:
            break;
        }

        break;

      default:
        break;
    }
  }

  throw HttpParseInvalidRequest();
}

int RequestHeader::ParseBodyLine(const std::string &data)
{
  if (this->pos_ == data.length()) {
    this->body = "";
    return WSV_OK;
  }

  if (wsv_str_4cmp_p(data, data.length() - 4, '\r', '\n', '\r', '\n')) {
    this->body = data.substr(this->pos_, data.length() - this->pos_ - 4);
    return WSV_OK;
  }

  throw HttpParseInvalidBody();
}

std::string RequestHeader::ToString()
{
  req_header_it_t it;
  std::string ret;
  ret += MethodToString() + " " + this->host + " " + HttpVersionToString() + CRLF;

  for (it = this->conf.begin() ; it != conf.end() ; ++it) {
    ret += it->second->key + ": " + it->second->value + CRLF;
  }
  ret += CRLF;
  if (body.length() != 0) {
    ret += body + CRLF;
    ret += CRLF;
  }
  return ret;
}

std::string RequestHeader::MethodToString()
{
  switch (this->method)
  {
    case HTTP_UNKNOWN:
      return "UNKNOWN";

    case HTTP_GET:
      return "GET";

    case HTTP_POST:
      return "POST";

    case HTTP_PUT:
      return "PUT";

    case HTTP_DELETE:
      return "DELETE";

    default:
      return "";
  }
}

std::string RequestHeader::HttpVersionToString()
{
  std::string charset[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
  return "HTTP/" + charset[this->http_major] + "." + charset[this->http_minor];
}

char** RequestHeader::ToCgi(const int &type) {
  const std::string php[4] = {
    "REQUEST_METHOD",
    "SERVER_PROTOCOL",
    "PATH_INFO",
    "CONTENT_LENGTH"
  };

  const std::string bla[2] = {
    "REQUEST_METHOD",
    "SERVER_PROTOCOL",
  };

  // const std::string* cgiHeader[2] = {
  //   php,
  //   bla,
  // };

  (void)type;
  std::string tmp;
  char **ret;
  int n, i;

  n = 4 + 1;
  ret = (char **)malloc(sizeof(char *) * n);
  for (i = 0 ; i < n - 1 ; ++i) {
    if (php[i] == "REQUEST_METHOD") {
      tmp = "REQUEST_METHOD=" + MethodToString();
    } else if (php[i] == "SERVER_PROTOCOL") {
      tmp = "SERVER_PROTOCOL=" + HttpVersionToString();
    } else if (php[i] == "PATH_INFO") {
      tmp = "PATH_INFO=../";
    } else if (php[i] == "CONTENT_LENGTH") {
      std::string value = GetItem("Content-Length").value;
      tmp = "CONTENT_LENGTH=" + (value == "" ? "0" : value);
    }
    ret[i] = strdup(tmp.c_str());
  }
  ret[i] = NULL;
  return ret;
}

void RequestHeader::Print()
{
  PrintRequestLine();
  PrintHeaderLine();
  PrintBodyLine();
}

void RequestHeader::PrintRequestLine()
{
  std::cout << COLOR_RED << "[ REQUEST LINE ]" << COLOR_DEFAULT << std::endl;
  std::cout << "  [method] : ";

  std::cout << std::endl;
  std::cout << "  [host] : " << host << std::endl;
  std::cout << "  [http version] : " << this->http_major << "." << this->http_minor << std::endl;
}

void RequestHeader::PrintHeaderLine()
{
  req_header_it_t it;

  std::cout << COLOR_GREEN << "[ HEADER LINE ]" << COLOR_DEFAULT << std::endl;
  for (it = this->conf.begin() ; it != this->conf.end() ; ++it) {
    std::cout << "  [" << it->second->key << "] : " << it->second->value << std::endl;
  }
}

void RequestHeader::PrintBodyLine()
{
  std::cout << COLOR_BLUE << "[ HEADER LINE ]" << COLOR_DEFAULT << std::endl;
  std::cout << body << std::endl;
}

