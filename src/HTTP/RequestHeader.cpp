#include "RequestHeader.hpp"

RequestHeader::RequestHeader() : method(0), pos_(0) {
  this->SetItem("_", "");
  this->host = WSV_STR_EMPTY;
  this->http_major = 0;
  this->http_minor = 0;
  this->state = 0;
}

RequestHeader::RequestHeader(const RequestHeader &origin)
{
  *this = origin;
}

RequestHeader::~RequestHeader() {
  for (req_header_it_t it = this->conf.begin(); it != this->conf.end(); ++it) {
    delete it->second;
  }
}

RequestHeader& RequestHeader::operator=(const RequestHeader &rv)
{
  this->method = rv.method;
  this->host = rv.host;
  this->http_major = rv.http_major;
  this->http_minor = rv.http_minor;
  this->body = rv.body;
  req_header_t temp = rv.conf;
  for (req_header_it_t it = temp.begin() ; it != temp.end() ; ++it) {
    this->SetItem(it->first, it->second->value);
  }
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

  SetItem(key, value);
}

void RequestHeader::SetItem(const std::string &key, const std::string &value)
{
  req_header_it_t it = FindItem(key);
  if (it != this->conf.end()) {
    delete it->second;
    this->conf.erase(it);
  }

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
  int contents_length;
  int pos;

  if (chunked.length() == 0 || chunked.find(CRLF) == std::string::npos) {
    return -1;
  }

  pos = chunked.find(CRLF);
  contents_length = ft_hextoi(chunked.substr(0, pos));
  if (contents_length == 0) return 0;
  this->body += chunked.substr(pos + 2, contents_length);
  return contents_length;
}

req_header_it_t RequestHeader::FindItem(const std::string &key)
{
  req_header_it_t it = this->conf.find(key);
  return it;
}

wsv_header_t& RequestHeader::GetItem(const std::string &key)
{
  req_header_it_t it = FindItem(key);
  if (it == this->conf.end()) {
    if (FindItem("_") == this->conf.end()) {
      this->SetItem("_", "");
    }
    return *(this->conf["_"]);
  }
  return *(it->second);
}
void  RequestHeader::Parse(const std::string &data)
{
  ParseRequestLine(data);
  ParseHeaderLine(data);
  ParseBodyLine(data);
  this->state = 1;
}

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
          throw HTTP_STATUS_NOT_IMPLEMENTED;
        }

        state = wsb_method;
        break;

      case wsb_method:
        if (ch == ' ') {
          switch (pos)
          {
            case 3:
              if (wsb_str_3cmp(data, 'G', 'E', 'T')) {
                this->method = HTTP_GET;
                break;
              }

              if (wsb_str_3cmp(data, 'P', 'U', 'T')) {
                this->method = HTTP_PUT;
                break;
              }
              break;

            case 4:
              if (wsb_str_4cmp(data, 'P', 'O', 'S', 'T')) {
                this->method = HTTP_POST;
                break;
              }
              if (wsb_str_4cmp(data, 'H', 'E', 'A', 'D')) {
                throw HTTP_STATUS_METHOD_NOT_ALLOWED;
              }
              break;

            case 5:
              if (wsb_str_5cmp(data, 'P', 'A', 'T', 'C', 'H')) {
                throw HTTP_STATUS_METHOD_NOT_ALLOWED;
              }

              if (wsb_str_5cmp(data, 'T', 'R', 'A', 'C', 'E')) {
                throw HTTP_STATUS_METHOD_NOT_ALLOWED;
              }
              break;

            case 6:
              if (wsb_str_6cmp(data, 'D', 'E', 'L', 'E', 'T', 'E')) {
                this->method = HTTP_DELETE;
              }
              break;

            case 7:
              if (wsb_str_7cmp(data, 'C', 'O', 'N', 'N', 'E', 'C', 'T')) {
                throw HTTP_STATUS_METHOD_NOT_ALLOWED;
              }
              if (wsb_str_7cmp(data, 'O', 'P', 'T', 'I', 'O', 'N', 'S')) {
                throw HTTP_STATUS_METHOD_NOT_ALLOWED;
              }
              break;

            default:
              break;
          }

          if (!this->method)
            throw HTTP_STATUS_NOT_IMPLEMENTED;

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
            this->host = ToLower(data.substr(host_start, host_end - host_start));
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
          pos += 7;
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
  size_t        key_start, key_end, value_start, value_end;
  std::string   key, value;
  u_char        ch;
  size_t        pos;

  enum {
    wsv_start = 0,
    wsv_header_key,
    wsv_header_value_before,
    wsv_header_value,
    wsv_invalid_key,
    wsv_invalid_key_newline,
    wsv_valide_key_newline,
    wsv_almost_done,
    wsv_done
  } state;

  state = wsv_start;

  for (pos = this->pos_ ; data[pos] ; ++pos) {
    ch = data[pos];

    switch (state) {
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

        if (isalpha(ch)) {
          key_start = pos;
          state = wsv_header_key;
        }
        break;

      case wsv_header_key:
        switch (ch) {
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
        if (ch != ' ' && isprint(ch)) {
          state = wsv_header_value;
          value_start = pos;
          break;
        }

        switch (ch) {
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
        switch (ch) {
          case CR:
            value_end = pos - 1;
            state = wsv_valide_key_newline;
            break;

          default:
            break;
        }
        break;

      case wsv_invalid_key:
        switch (ch) {
          case CR:
            state = wsv_invalid_key_newline;
            break;

          default:
            break;
        }
        break;

      case wsv_invalid_key_newline:
        switch (ch) {
        case LF:
          state = wsv_almost_done;
          break;

        default:
          break;
        }
        break;

      case wsv_valide_key_newline:
        switch (ch) {
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
        switch (ch) {
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
        switch (ch) {
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
  switch (this->method) {
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

char** RequestHeader::ToCgi(const std::string &path) {
  std::string tmp;
  char        **ret;
  size_t      len, i;

  std::vector<std::string> cgi;

  cgi.push_back("REQUEST_METHOD");
  cgi.push_back("SERVER_PROTOCOL");
  cgi.push_back("PATH_INFO");
  cgi.push_back("CONTENT_LENGTH");


  for (req_header_it_t it = this->conf.begin() ; it != this->conf.end() ; ++it) {
    if (wsb_str_2cmp(it->first, 'X', '-')) {
      cgi.push_back("HTTP_" + it->second->key + "=" + it->second->value);
    }
  }

  len = cgi.size() + 1;
  ret = (char **)malloc(sizeof(char *) * len);

  for (i = 0 ; i < len - 1 ; ++i) {
    if (cgi[i] == "REQUEST_METHOD") {
      tmp = "REQUEST_METHOD=" + MethodToString();
    } else if (cgi[i] == "SERVER_PROTOCOL") {
      tmp = "SERVER_PROTOCOL=" + HttpVersionToString();
    } else if (cgi[i] == "PATH_INFO") {
      tmp = "PATH_INFO=" + path;
    } else if (cgi[i] == "CONTENT_LENGTH") {
      std::string value = ft_itos(body.size());
      tmp = "CONTENT_LENGTH=" + (value == "" ? "0" : value);
    } else {
      ret[i] = strdup(cgi[i].c_str());
      continue;
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
  std::cout << COLOR_CYAN << "[ REQUEST LINE ]" << COLOR_DEFAULT << std::endl;
  std::cout << "  [method] : " << MethodToString() << std::endl;
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

void RequestHeader::Clear()
{
  this->method = 0;
  this->host = WSV_STR_EMPTY;
  this->http_major = 0;
  this->http_minor = 0;
  this->state = 0;
  for (req_header_it_t it = this->conf.begin(); it != this->conf.end(); ++it) {
    delete it->second;
  }
  this->conf.clear();
  this->SetItem("_", "");
  this->pos_ = 0;
}
