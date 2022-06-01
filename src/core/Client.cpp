#include "Client.hpp"
#include "utils.hpp"
#include "NotFoundError.hpp"

Client::Client(KQueue &kq, int fd, Server *server) : FdInterface(kq, kFdClient, fd), server(server)
{
  request = nullptr;
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
  int n = write(interface_fd, response_message.c_str(), response_message.size());
  if (n <= 0)
    return n;
  response_message = response_message.substr(n);
  n = response_message.size();

  return n;
}

int IsCRLF(const std::string &request_message)
{
  return (request_message.find(CRLF) != std::string::npos);
}

int Client::CheckCgi()
{
  // TODO : CGI 처리를 해야하는지 확인하는 부분
  return 0;
}

FdInterfaceType Client::ParseHeader(std::string &request_message)
{
  std::string tmp = request_message.substr(request_message.find(CRLF) + 4);
  request_message = request_message.substr(0, request_message.find(CRLF));
  request = new RequestHeader(request_message);
  request_message = tmp;

  if (request->FindItem("Content-Length")->first != ""
    && atoi(request->GetItem("Content-Length").value.c_str()) > 0
    && request->body.size() <= 0)
    return kFdClient;
  else if(CheckCgi()) {
    return kFdCgi;
  }
  else if(request->host != "") {
    return kFdFileio;
  }
  else
    return kFdNone;
}

FdInterfaceType Client::ParseBody(std::string &request_message)
{
  std::string tmp = request_message.substr(request_message.find(CRLF) + 4);
  request_message = request_message.substr(0, request_message.find(CRLF));

  if (request->FindItem("Transfer-Encoding")->first != ""
    && request->FindItem("Transfer-Encoding")->second->value == "chunked") {
    int status = request->SetChunked(request_message);
    request_message = tmp;
    if (status > 0)
      return kFdClient;
    else if (status == 0 && CheckCgi())
      return kFdCgi;
    else if (status == 0 && request->host != "")
      return kFdFileio;
    else
      return kFdNone;
  }
  else {
    request->SetBody(request_message);
    request_message = tmp;
    if (CheckCgi())
      return kFdCgi;
    else if (request->host != "")
      return kFdFileio;
    else
      return kFdNone;
  }
}

FdInterfaceType Client::ParseReq()
{
  if (!IsCRLF(request_message))
    return kFdNone;

  if (request == nullptr)
    return ParseHeader(request_message);
  else
    return ParseBody(request_message);
}

const std::string Client::GetFilePath() const
{
  // TODO : 파일 경로를 반환하는 부분
  std::string path = "." + request->host;
  return path;
}
