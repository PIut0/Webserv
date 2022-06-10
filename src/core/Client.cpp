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
  if (request)
    delete request;
  if (response)
    delete response;
  if (interface_fd > 2)
    close(interface_fd);
}

int Client::EventRead()
{
  char buf[1024];
  memset(buf, 0, sizeof(buf));
  int n = read(interface_fd, buf, sizeof(buf) - 1);
  if (n <= 0) {
    return n;
  }	// n == 0: 클라이언트에서 close & n == -1: 클라이언트 프로세스가 종료됨
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
  // request->host 의 . 뒷부분을 locationblock cgi_info에서 찾아서 있으면 OK
  size_t pos;
  cgiinfo_it_t it;
  LocationBlock *locationBlock;
  std::string extension;

  if ((pos = this->request->host.find_last_of('.')) == std::string::npos){
    return 0;
  }

  extension = this->request->host.substr(pos);
  locationBlock = GetLocationBlock();
  if ((it = locationBlock->cgi_info.find(extension)) == locationBlock->cgi_info.end()) {
    return 0;
  }

  return 1;
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
  } catch(const HttpParseInvalidRequest& e) {
    return 400;
  } catch(const NotAllowedError& e) {
    return 405;
  } catch(const HttpParseInvalidResponse& e) {
    return 500;
  } catch(const NotImplementedError& e) {
    return 501;
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
  int status = CheckRequest();
  LocationBlock *loc = GetLocationBlock();

  std::cout << request->ToString() << std::endl;

  if (status == 0 && loc->ret != "") {
    status = ft_stoi(loc->ret.substr(0, 3));

    if (loc->ret.size() > 4) {
      std::string ret = loc->ret.substr(4);
      ret = trim(ret);
      if (ret.size() > 0)
        response->SetItem("Location", ret);
    }
  }

  if (status) {
    response->SetItem("Status", StatusCode(status));
    if (loc && loc->error_page != "")
      return kFdGetMethod;
    else {
      response->body = DefaultErrorPage(status);
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
  else if(request->host != "" && request->method == HTTP_POST)
    return kFdPostMethod;
  else if(request->host != "" && request->method == HTTP_DELETE)
    return kFdDeleteMethod;
  else
    return kFdNone;
}

FdInterfaceType Client::ParseBody()
{
  std::string req;
  int content_length;

  if (request->GetItem("Transfer-Encoding").value == "chunked") {
    // TODO : chunked 데이터 파싱 추후 확정필요
    //if (!IsCRLF(request_message))
    //  return kFdNone;
    size_t pos;
    if ((pos = FindSecondCRLF(request_message)) == std::string::npos)
      return kFdNone;

    req = request_message.substr(0, pos);
    request_message = request_message.substr(pos + 2);

    int chunked_status = request->SetChunked(req);
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

    if((content_length = ft_stoi(request->GetItem("Content-Length").value)) > 0) {
      request->SetBody(request->body + req);
      if (request->body.length() < static_cast<unsigned long>(content_length))
        return kFdNone;
      request->SetBody(request->body.substr(0, content_length));
    }
  }

  size_t max_body_size = GetLocationBlock()->request_max_body_size;
  if (request->body.size() > max_body_size)
    response->SetItem("Status", StatusCode(413));

  if (CheckCgi())
    return kFdCgi;
  else if (request->host != "" && request->method == HTTP_GET)
    return kFdGetMethod;
  else if (request->host != "" && request->method == HTTP_PUT)
    return kFdPutMethod;
  else if (request->host != "" && request->method == HTTP_POST)
    return kFdPostMethod;
  else if (request->host != "" && request->method == HTTP_DELETE)
    return kFdDeleteMethod;
  else
    return kFdNone;
}

FdInterfaceType Client::ParseReq()
{
  //std::cout << "ParseReq: " << request_message << std::endl;
  FdInterfaceType type;

  if (request == nullptr) {
    if (!IsCRLF(request_message))
      return kFdNone;

    type = ParseHeader();

    if (request_message.size() <= 0)
      return type;
  }


  return ParseBody();
}

const std::string Client::GetFilePath()
{
  std::string path;

  int location_index = server->server_block.GetLocationBlockByPath(request->host);

  if (location_index == -1)
    throw NotFoundError(); // TODO : Catch 추가
  path = GetLocationBlock()->root
    + request->host.substr(request->host.find(GetLocationBlock()->location_path) + GetLocationBlock()->location_path.size());

  return path;
}

void Client::SetResponseMessage()
{
  if (response->status_code == "") {
    if (response->body.size() > 0)
      response->SetItem("Status", StatusCode(200));
    else
      response->SetItem("Status", StatusCode(204));
  }

  if (ft_stoi(response->status_code) >= 400)
    response->SetItem("Content-Type", "text/html");

  response->SetItem("Content-Length", ft_itos(response->body.size()));

  if (response->body.size() > 0) {
    if (response->FindItem("Content-Type") == response->conf.end()) {
      if (request && request->host.size() && request->host.find_last_of(".") != std::string::npos)
        response->SetItem("Content-Type", MimeType(request->host.substr(request->host.find_last_of(".") + 1)));
      else
        response->SetItem("Content-Type", "application/octet-stream");
    }
  }

  if (request && request->FindItem("Connection") != request->conf.end())
    response->SetItem("Connection", request->FindItem("Connection")->second->value);
  else if (response->FindItem("Connection") == response->conf.end())
    response->SetItem("Connection", "keep-alive");

  response->SetItem("Server", client->server->server_block.server_name);
  response->SetItem("Date", GetDate());
}
