#include "ResponseHeader.hpp"

ResponseHeader::ResponseHeader()
{
  this->status_code = "";
  this->status_msg  = "";
}

ResponseHeader::ResponseHeader(const std::string &data)
{
  this->pos_ = 0;

  ParseHeaderLine(data);
  ParseBody(data);
}

ResponseHeader::~ResponseHeader() {}

ResponseHeader& ResponseHeader::operator=(const ResponseHeader &rv)
{
  this->conf = rv.conf;
  return *this;
}

void ResponseHeader::SetBody(const std::string &body)
{
  this->body = body;
}

void ResponseHeader::SetItem(const std::string &line)
{
  size_t index = line.find(":");
  std::string key = line.substr(0, index);
  std::string value = line.substr(index + 2, line.length() - 1);
  // std::string trim_value = trim(value);

  SetItem(key, value);
}

void ResponseHeader::SetItem(const std::string &key, const std::string &value)
{
  if (key == "Status") {
    if (this->status_code != "") throw AlreadyExistKey();

    this->status_code = value.substr(0, 3);
    this->status_msg = value.substr(4);
    return ;
  }

  if (FindItem(key) != this->conf.end())
    throw AlreadyExistKey();

  wsv_header_t *el = new wsv_header_t();
  el->key = key;
  el->value = value;
  this->conf.push_back(el);
}

res_header_it_t ResponseHeader::FindItem(const std::string &key)
{
  res_header_it_t it;

  for (it = this->conf.begin() ; it != this->conf.end() ; ++it) {
    if ((*it)->key == key) break;
  }
  return it;
}

wsv_header_t& ResponseHeader::GetItem(const std::string &key)
{
  return *(*FindItem(key));
}

int ResponseHeader::ParseHeaderLine(const std::string &data)
{
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

  for (pos = this->pos_ ; (data)[pos] ; ++pos) {
    ch = (data)[pos];

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
            key = (data).substr(key_start, key_end - key_start + 1);
            value = (data).substr(value_start, value_end - value_start + 1);
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
  throw HttpParseInvalidResponse();
}

int ResponseHeader::ParseBody(const std::string &data)
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

std::string ResponseHeader::ToString()
{
  std::string ret = "";

  ret += "HTTP/1.1 " + this->status_code + " " + this->status_msg + CRLF;
  for (size_t i = 0 ; i < conf.size() ; ++i) {
    ret += conf[i]->key + ": " + conf[i]->value + CRLF;
  }
  ret += CRLF;
  if (this->body.length() != 0) {
    ret += body + CRLF;
    ret += CRLF;
  }
  return ret;
}

void ResponseHeader::Print()
{
  PrintHeaderLine();
  PrintBody();
}

void ResponseHeader::PrintHeaderLine()
{
  std::cout << COLOR_GREEN << "[ HEADER LINE ]" << COLOR_DEFAULT << std::endl;
  std::cout << "  Status:  " + this->status_code + " " + this->status_msg << std::endl;
  for (size_t i = 0 ;  i < conf.size() ; ++i) {
    std::cout << "  " << conf[i]->key << ": " << conf[i]->value << std::endl;
  }
}

void ResponseHeader::PrintBody()
{
  std::cout << COLOR_BLUE << "[ BODY LINE ]" << COLOR_DEFAULT << std::endl;
  std::cout << body << std::endl;
}
