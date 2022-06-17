#include "Method.hpp"
#include "utils.hpp"

int IsDir(const std::string &path)
{
  return (path.back() == '/');
}

Method::Method(const Method &other) : FdInterface(other.kq, kFdNone, other.interface_fd), client(other.client)
{
  *this = other;
}

Method &Method::operator=(const Method &other)
{
  if (this != &other) {
    this->kq = other.kq;
  }
  return *this;
}

Method::Method(Client &client) : FdInterface(client.kq, kFdNone, 0), client(client)
{
  this->from_cgi[0] = 0;
  this->from_cgi[1] = 0;
  this->to_cgi[0] = 0;
  this->to_cgi[1] = 0;
  this->interface_fd = 0;
  this->target_fd = 0;
  this->read_data = "";
  this->cgi_write_data_idx = 0;
  this->cgi_write_data_size = 0;
  this->cgi_write_data = "";
  this->write_data_idx = 0;
  this->write_data_size = 0;
  this->write_data = "";
  this->target_path = "";
  this->location = nullptr;
}

Method::~Method()
{
  // request.clear();
  // response.clear();
  CloseFd(from_cgi[0]);
  CloseFd(from_cgi[1]);
  CloseFd(to_cgi[0]);
  CloseFd(to_cgi[1]);
  CloseFd(interface_fd);
}

void Method::SetMethod(FdInterfaceType type)
{
  interface_type = type;

  read_data = WSV_STR_EMPTY;
  write_data = WSV_STR_EMPTY;
  cgi_write_data = WSV_STR_EMPTY;
  target_fd = client.interface_fd;

  location = client.GetLocationBlock();
  target_path = client.GetFilePath();

  try {
    if (ft_stoi(client.response.status_code) >= 400)
      throw ft_stoi(client.response.status_code);
    switch (type)
    {
      case kFdGetMethod:
        GetMethod(this);
        break;

      case kFdPutMethod:
        PutMethod(this);
        break;

      case kFdPostMethod:
        PostMethod(this);
        break;

      case kFdDeleteMethod:
        DeleteMethod(this);
        break;

      case kFdCgi:
        CgiMethod(this);
        break;

      default:
        break;
    }
  } catch (int status) {
    SetResponseStatus(status);
    ResponseErrorPage();
  }

}

void Method::Clear()
{
  CloseFd(from_cgi[0]);
  CloseFd(from_cgi[1]);
  CloseFd(to_cgi[0]);
  CloseFd(to_cgi[1]);
  CloseFd(interface_fd);

  interface_type = kFdNone;
  interface_fd = 0;

  read_data = WSV_STR_EMPTY;
  write_data = WSV_STR_EMPTY;
  cgi_write_data = WSV_STR_EMPTY;

  from_cgi[0] = 0;
  from_cgi[1] = 0;
  to_cgi[0] = 0;
  to_cgi[1] = 0;
  interface_fd = 0;
  target_fd = 0;

  read_data = "";
  cgi_write_data_idx = 0;
  cgi_write_data_size = 0;
  cgi_write_data = "";
  write_data_idx = 0;
  write_data_size = 0;
  write_data = "";
  target_path = "";

  client.request.Clear();
  client.response.Clear();

  location = NULL;
  target_path = WSV_STR_EMPTY;
}

int Method::EventRead()
{
  client.SetSocketHitTime();
  char buf[BUFFER_SIZE + 1];
  memset(buf, 0, BUFFER_SIZE + 1);
  int n = read(interface_fd, buf, BUFFER_SIZE);
  buf[n] = '\0';
  read_data += buf;

  return IsEOF(interface_fd);
}

int Method::EventWrite()
{
  client.SetSocketHitTime();
  if (write_data_idx == 0) {
    write_data = client.response.ToString();
    write_data_size = write_data.size();
  }

  int len = write_data_size - write_data_idx > BUFFER_SIZE ? BUFFER_SIZE : write_data_size - write_data_idx;
  int n = write(target_fd, write_data.c_str() + write_data_idx, len);

  if (n <= 0)
    return n;

  write_data_idx += n;
  if (write_data_size - write_data_idx <= 0) {
    client.response.body = WSV_STR_EMPTY;
    std::cout << "- Response -" << std::endl << client.response.ToString() << std::endl;
    std::cout << "write_data_size: " << write_data_size << std::endl;
    std::cout << "write_data_idx: " << write_data_idx << std::endl;
  }
  return write_data_size - write_data_idx;
}

int Method::FileWrite()
{
  client.SetSocketHitTime();
  if (cgi_write_data_idx == 0) {
    cgi_write_data_size = client.request.body.size();
  }

  int len = cgi_write_data_size - cgi_write_data_idx > BUFFER_SIZE ? BUFFER_SIZE : cgi_write_data_size - cgi_write_data_idx;
  int n = write(interface_fd, client.request.body.c_str() + cgi_write_data_idx, len);

  if (n <= 0)
    return n;

  cgi_write_data_idx += n;
  return cgi_write_data_size - cgi_write_data_idx;
}

void Method::SetResponseStatus(const int code)
{
  client.response.SetItem("Status", StatusCode(code));
}

void Method::ResponseErrorPage()
{
  if (!location || (interface_fd = open(location->error_page.c_str(), O_RDONLY)) < 0) {
    read_data = DefaultErrorPage(ft_stoi(client.response.status_code));
    SetResponseMessage();
    kq->AddEvent(target_fd, EVFILT_WRITE, this);
  }
  else {
    if (!IsEOF(interface_fd)) {
      SetResponseMessage();
      kq->AddEvent(target_fd, EVFILT_WRITE, this);
    }
    else {
      fcntl(interface_fd, F_SETFL, O_NONBLOCK);
      kq->AddEvent(interface_fd, EVFILT_READ, this);
    }
  }
}

void Method::SetResponseMessage()
{
  if (client.response.body.size() <= 0)
    client.response.SetBody(read_data);

  if (client.response.status_code == "") {
    if (client.response.body.size() > 0)
      SetResponseStatus(200);
    else
      SetResponseStatus(204);
  }

  if (ft_stoi(client.response.status_code) >= 400)
    client.response.SetItem("Content-Type", "text/html");

  client.response.SetItem("Content-Length", ft_itos(client.response.body.size()));

  if (client.response.body.size() > 0) {
    if (client.response.FindItem("Content-Type") == client.response.conf.end()) {
      if (client.request.host.size() && client.request.host.find_last_of(".") != std::string::npos) {
        client.response.SetItem("Content-Type", MimeType(client.request.host.substr(client.request.host.find_last_of(".") + 1)));
      } else {
        client.response.SetItem("Content-Type", "application/octet-stream");
      }
    }
  }

  if (client.request.FindItem("Connection") != client.request.conf.end()) {
    client.response.SetItem("Connection", client.request.FindItem("Connection")->second->value);\
  } else if (client.response.FindItem("Connection") == client.response.conf.end()) {
    client.response.SetItem("Connection", "keep-alive");
  }

  client.response.SetItem("Server", client.server->server_block.server_name);
  client.response.SetItem("Date", GetDate());
}
