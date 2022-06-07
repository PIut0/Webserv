#include "Client.hpp"
#include "utils.hpp"

Client::Client(KQueue &kq, int fd, Server *server) : FdInterface(kq, kFdClient, fd), server(server)
{
  request = nullptr;
  response = nullptr;
  fcntl(interface_fd, F_SETFL, O_NONBLOCK);
  kq.AddEvent(interface_fd, EVFILT_READ, this);
}

Client::~Client()
{
  close(interface_fd);
}

int Client::EventRead()
{
  char buf[1024];
  int n = read(interface_fd, buf, sizeof(buf) - 1);
  if (n <= 0)	// n == 0: 클라이언트에서 close & n == -1: 클라이언트 프로세스가 종료됨
    return n;
  buf[n] = '\0';
  request_message += buf;

  return n;
}

int Client::EventWrite()
{
  std::string res = response->ToString();
  int n = write(interface_fd, res.c_str(), res.size());
  if (n <= 0)
    return n;
  res = res.substr(n);
  n = res.size();

  return n;
}

int IsCRLF(const std::string &request_message)
{
  return (request_message.find(D_CRLF) != std::string::npos);
}

LocationBlock *Client::GetLocationBlock()
{
  if (request == nullptr)
    return nullptr;
  if (server->server_block.GetLocationBlockByPath(request->host) < 0)
    return nullptr;
  return &(server->server_block.location[server->server_block.GetLocationBlockByPath(request->host)]);
}

int Client::CheckCgi()
{
  // TODO : CGI 처리를 해야하는지 확인하는 부분
  return 0;
}

int Client::CheckRequest()
{
  //std::cout << request_message << std::endl;
  std::string req = request_message.substr(0, request_message.find(D_CRLF) + 4);
  request_message = request_message.substr(request_message.find(D_CRLF) + 4);
  try {
    response = new ResponseHeader();
    request = new RequestHeader();
    request->Parse(req);
  } catch(const HttpParseInvalidBody& e) {
    return 400;
  } catch(const HttpParseInvalidMethod& e) {
    return 405;
  } catch(const HttpParseInvalidRequest& e) {
    return 400;
  } catch(const HttpParseInvalidResponse& e) {
    return 0;
  }

  if (request->HttpVersionToString() != "HTTP/1.1")
    return 505;

  if (!request->method || request->host.size() <= 0 || request->host[0] != '/')
    return 400;

  LocationBlock *location_block = GetLocationBlock();
  if (!(location_block->allow_methods & request->method))
    return 405;

  return 0;
}

FdInterfaceType Client::ParseHeader()
{
  int status = CheckRequest();
  LocationBlock *loc = GetLocationBlock();

  if (status == 0 && loc->ret != "") {
    std::cout << "ret: " << loc->ret << std::endl;
    status = ft_stoi(loc->ret.substr(0, 3));

    if (loc->ret.size() > 4) {
      std::string red = loc->ret.substr(4);
      red = rtrim(ltrim(red));
      if (red.size() > 0)
        response->SetItem("Location", red);
    }
  }

  if (status) {
    response->SetItem("Status", StatusCode(status));
    if (loc && loc->error_page != "")
      return kFdGetMethod;
    else {
      response->body = "";
      return kFdClient;
    }
  }

  if (ft_stoi(request->GetItem("Content-Length").value) > 0 && request->body.size() <= 0)
    return kFdNone;
  else if(request->GetItem("Transfer-Encoding").value == "chunked")
    return kFdNone;
  else if(CheckCgi())
    return kFdCgi;
  else if(request->host != "" && request->method == HTTP_GET)
    return kFdGetMethod;
  else if(request->host != "" && request->method == HTTP_PUT)
    return kFdPutMethod;
  else
    return kFdNone;
}

FdInterfaceType Client::ParseBody()
{
  std::string req;
  int content_length;

  if (IsCRLF(request_message)) {
    req = request_message.substr(0, request_message.find(D_CRLF));
    request_message = request_message.substr(request_message.find(D_CRLF) + 4);
  } else {
    req = request_message;
    request_message = "";
  }

  if (request->GetItem("Transfer-Encoding").value == "chunked") {
    if (request->SetChunked(req) != 0)
      return kFdNone;
  }

  else if((content_length = ft_stoi(request->GetItem("Content-Length").value)) > 0) {
    request->SetBody(request->body + req);
    if (request->body.length() < static_cast<unsigned long>(content_length))
      return kFdNone;
    request->SetBody(request->body.substr(0, content_length));
  }

  if (CheckCgi())
    return kFdCgi;
  else if (request->host != "" && request->method == HTTP_GET)
    return kFdGetMethod;
  else if (request->host != "" && request->method == HTTP_PUT)
    return kFdPutMethod;
  else
    return kFdNone;
}

FdInterfaceType Client::ParseReq()
{
  if (request == nullptr) {
    if (!IsCRLF(request_message))
      return kFdNone;
    return ParseHeader();
  }
  else
    return ParseBody();
}

const std::string Client::GetFilePath()
{
  std::string path;

  if (response->status_code != "") {
    request->SetHost(GetLocationBlock()->error_page);
    path = request->host;
  }

  else {
    int location_index = server->server_block.GetLocationBlockByPath(request->host);

    if (location_index == -1)
      throw NotFoundError();
    path = GetLocationBlock()->root
      + request->host.substr(request->host.find(GetLocationBlock()->location_path) + GetLocationBlock()->location_path.size());
  }

  return path;
}

void Client::SetResponseMessage()
{
  if (response->status_code == "")
    response->SetItem("Status", StatusCode(200));

  response->SetItem("Content-Length", ft_itos(response->body.size()));

  if (response->body.size() > 0) {
    if (response->FindItem("Content-Type") == response->conf.end()) {
      if (request && request->host.size() && request->host.find_last_of(".") != std::string::npos)
        response->SetItem("Content-Type", MimeType(request->host.substr(request->host.find_last_of(".") + 1)));
      else
        response->SetItem("Content-Type", "application/octet-stream");
    }
  }

  if (request && request->FindItem("Connection")->first == "Connection")
    response->SetItem("Connection", request->FindItem("Connection")->second->value);
  else
    response->SetItem("Connection", "keep-alive");
}
