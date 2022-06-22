#include "ResponseHeader.hpp"

ResponseHeader::ResponseHeader()
{
  this->SetItem("_", "");
  this->status_code = WSV_STR_EMPTY;
  this->status_msg  = WSV_STR_EMPTY;
  this->state = 0;
  this->pos_ = 0;
}

ResponseHeader::ResponseHeader(const ResponseHeader &origin)
{
  *this = origin;
}

ResponseHeader::~ResponseHeader() {
  for (res_header_it_t it = this->conf.begin(); it != this->conf.end(); ++it) {
    delete *it;
  }
}

ResponseHeader& ResponseHeader::operator=(const ResponseHeader &rv)
{
  this->status_code = rv.status_code;
  this->status_msg = rv.status_msg;
  this->body = rv.body;
  for (size_t i = 0 ; i < rv.conf.size() ; ++i) {
    this->SetItem(rv.conf[i]->key, rv.conf[i]->value);
  }
  this->pos_ = rv.pos_;
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

  SetItem(key, value);
}

void ResponseHeader::SetItem(const std::string &key, const std::string &value)
{
  if (key == "Status") {
    this->status_code = value.substr(0, 3);
    this->status_msg = value.substr(4);
    return ;
  }
  res_header_it_t it = FindItem(key);
  if (it != this->conf.end()) {
    delete *it;
    this->conf.erase(it);
  }

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
  res_header_it_t it = FindItem(key);
  if (it == this->conf.end()) {
    if (FindItem("_") == this->conf.end()) {
      this->SetItem("_", "");
    }
    return *(*FindItem("_"));
  }
  return *(*it);
}

void ResponseHeader::Parse(const std::string &data)
{
  ParseHeaderLine(data);
  ParseBody(data);
}

int ResponseHeader::ParseHeaderLine(const std::string &data)
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

  for (pos = this->pos_ ; (data)[pos] ; ++pos) {
    ch = (data)[pos];

    switch (state) {
      case wsv_start:
        if ((ch < 'A' || ch > 'Z') && ch != '_' && ch != '-') {
          state = wsv_invalid_key;
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
            key = (data).substr(key_start, key_end - key_start + 1);
            value = (data).substr(value_start, value_end - value_start + 1);
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
  throw HttpParseInvalidResponse();
}

int ResponseHeader::ParseBody(const std::string &data)
{
  if (this->pos_ == data.length()) {
    this->body = WSV_STR_EMPTY;
  } else {
    this->body = data.substr(this->pos_, data.length() - this->pos_);
  }
  return WSV_OK;
}

std::string ResponseHeader::ToString()
{
  std::string ret = WSV_STR_EMPTY;

  ret += "HTTP/1.1 " + this->status_code + " " + this->status_msg + CRLF;
  for (size_t i = 0 ; i < conf.size() ; ++i) {
    ret += conf[i]->key + ": " + conf[i]->value + CRLF;
  }
  ret += CRLF;
  if (this->body.length() != 0) {
    ret += body;
  }
  return ret;
}

void ResponseHeader::Print()
{
  PrintHeaderLine();
  PrintBody();
}

void ResponseHeader::PrintResponseLine()
{
  std::cout << COLOR_MAGENTA << "[ Response LINE ]" << COLOR_DEFAULT << std::endl;
  std::cout << "  Status:  " + this->status_code + " " + this->status_msg << std::endl;
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

void ResponseHeader::Clear()
{
  this->status_code = WSV_STR_EMPTY;
  this->status_msg = WSV_STR_EMPTY;
  this->body = WSV_STR_EMPTY;
  this->state = 0;
  this->pos_ = 0;
  for (res_header_it_t it = this->conf.begin(); it != this->conf.end(); ++it) {
    delete *it;
  }
  this->conf.clear();
  this->SetItem("_", "");
}
