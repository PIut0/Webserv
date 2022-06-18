#include "Client.hpp"
#include "utils.hpp"

Client &Client::operator=(const Client &other)
{
  this->interface_fd = other.interface_fd;
  this->interface_type = other.interface_type;
  this->kq = other.kq;
  this->cur_method = other.cur_method;
  this->server = other.server;
  this->request = other.request;
  this->response = other.response;
  this->request_message = other.request_message;
  this->socketHitTime = other.socketHitTime;
  return (*this);
}

Client::Client() : FdInterface(nullptr, kFdNone), cur_method(*this), server(NULL)
{
  SetSocketHitTime();
}

Client::Client(const Client &other) : FdInterface(other.kq, kFdClient, other.interface_fd), cur_method(*this), server(other.server)
{
  *this = other;
}

Client::Client(KQueue *kq, int fd, Server *server) : FdInterface(kq, kFdClient, fd), cur_method(*this), server(server)
{
  SetSocketHitTime();
}

Client::~Client()
{
  CloseFd(interface_fd);
}


int Client::EventRead()
{
  SetSocketHitTime();
  char buf[BUFFER_SIZE + 1];
  memset(buf, 0, BUFFER_SIZE + 1);
  int n = read(interface_fd, buf, BUFFER_SIZE);
  if (n <= 0) {
    return n;
  }
  buf[n] = '\0';
  request_message += buf;

  return n;
}

int Client::EventWrite()
{
  return 1;
}

int IsCRLF(const std::string &request_message)
{
  return (request_message.find(D_CRLF) != std::string::npos);
}

LocationBlock *Client::GetLocationBlock()
{
  return &(server->server_block.location[server->server_block.GetLocationBlockByPath(request.host)]);
}

int Client::CheckCgi()
{
  size_t pos;
  cgiinfo_it_t it;
  LocationBlock *locationBlock;
  std::string extension;

  if ((pos = this->request.host.find_last_of('.')) == std::string::npos){
    return 0;
  }

  extension = this->request.host.substr(pos);
  locationBlock = GetLocationBlock();
  if ((it = locationBlock->cgi_info.find(extension)) == locationBlock->cgi_info.end()) {
    return 0;
  }

  return 1;
}

int Client::CheckRequest()
{
  std::string req = request_message.substr(0, request_message.find(D_CRLF) + 4);
  request_message = request_message.substr(request_message.find(D_CRLF) + 4);

  response.Clear();
  request.Clear();
  request.Parse(req);

  if (request.HttpVersionToString() != "HTTP/1.1")
    throw 505;

  if (!request.method || request.host.size() <= 0 || request.host[0] != '/')
    throw 400;

  return 0;
}

size_t FindSecondCRLF(std::string &request_message)
{
  size_t pos = request_message.find(CRLF);
  if (pos == std::string::npos)
    return pos;
  pos = request_message.find(CRLF, pos + 2);
  if (pos == std::string::npos)
    return pos;
  return pos;
}

FdInterfaceType Client::ParseHeader()
{
  try {
    int status = CheckRequest();
    LocationBlock *loc = GetLocationBlock();

    std::cout << "- Request -" << std::endl << request.ToString() << std::endl;

    if (status == 0 && loc->ret != "") {
      status = ft_stoi(loc->ret.substr(0, 3));

      if (loc->ret.size() > 4) {
        std::string ret = loc->ret.substr(4);
        ret = trim(ret);
        if (ret.size() > 0)
          response.SetItem("Location", ret);
      }
      throw status;
    }

    if (ft_stoi(request.GetItem("Content-Length").value) > 0 && request.body.size() <= 0) {
      return kFdNone;
    } else if(request.GetItem("Transfer-Encoding").value == "chunked") {
      return kFdNone;
    }

    if (!(loc->allow_methods & request.method)) {
      response.SetItem("Status", StatusCode(405));
      return kFdGetMethod;
    }

    if(CheckCgi()) {
      return kFdCgi;
    } else if(request.host != "" && request.method == HTTP_GET) {
      return kFdGetMethod;
    } else if(request.host != "" && request.method == HTTP_PUT) {
      return kFdPutMethod;
    } else if(request.host != "" && request.method == HTTP_POST) {
      return kFdPostMethod;
    } else if(request.host != "" && request.method == HTTP_DELETE) {
      return kFdDeleteMethod;
    } else {
      return kFdNone;
    }
  }
  catch(int status) {
    response.SetItem("Status", StatusCode(status));
    return kFdGetMethod;
  }
}

FdInterfaceType Client::ParseBody()
{
  try {
    std::string req;
    int content_length;

    if (request.GetItem("Transfer-Encoding").value == "chunked") {
      size_t pos;
      if ((pos = FindSecondCRLF(request_message)) == std::string::npos)
        return kFdNone;

      req = request_message.substr(0, pos);
      request_message = request_message.substr(pos + 2);

      int chunked_status = request.SetChunked(req);
      if (chunked_status && FindSecondCRLF(request_message) != std::string::npos)
        return ParseBody();
      if (chunked_status != 0)
        return kFdNone;
    }

    else {
      if (IsCRLF(request_message)) {
        req = request_message.substr(0, request_message.find(D_CRLF));
        request_message = request_message.substr(request_message.find(D_CRLF) + 4);
      } else {
        req = request_message;
        request_message = "";
      }

      if((content_length = ft_stoi(request.GetItem("Content-Length").value)) > 0) {
        request.SetBody(request.body + req);
        if (request.body.length() < static_cast<unsigned long>(content_length))
          return kFdNone;
        request.SetBody(request.body.substr(0, content_length));
      }
    }

    LocationBlock *loc = GetLocationBlock();
    size_t max_body_size = loc->request_max_body_size;

    if (request.body.size() > max_body_size)
      response.SetItem("Status", StatusCode(413));

    if (!(loc->allow_methods & request.method))
      response.SetItem("Status", StatusCode(405));

    if (CheckCgi()) {
      return kFdCgi;
    } else if (request.host != "" && request.method == HTTP_GET) {
      return kFdGetMethod;
    } else if (request.host != "" && request.method == HTTP_PUT) {
      return kFdPutMethod;
    } else if (request.host != "" && request.method == HTTP_POST) {
      return kFdPostMethod;
    } else if (request.host != "" && request.method == HTTP_DELETE) {
      return kFdDeleteMethod;
    } else {
      return kFdNone;
    }
  }
  catch (int status) {
    response.SetItem("Status", StatusCode(status));
    return kFdGetMethod;
  }
}

FdInterfaceType Client::ParseReq()
{
  FdInterfaceType type;

  if (!request.state) {
    if (!IsCRLF(request_message))
      return kFdNone;

    type = ParseHeader();

    if (response.status_code != "" || request_message.size() <= 0)
      return type;
  }

  return ParseBody();
}

const std::string Client::GetFilePath()
{
  std::string path;

  if (response.status_code != "")
    return path;

  int location_index = server->server_block.GetLocationBlockByPath(request.host);

  if (location_index == -1)
    throw HTTP_STATUS_NOT_FOUND;
  path = GetLocationBlock()->root
    + request.host.substr(request.host.find(GetLocationBlock()->location_path) + GetLocationBlock()->location_path.size());

  return path;
}

void Client::SetSocketHitTime()
{
  this->socketHitTime = clock();
}

void Client::Clear()
{
  this->server = nullptr;
  this->request.Clear();
  this->response.Clear();
  this->interface_fd = 0;
}
