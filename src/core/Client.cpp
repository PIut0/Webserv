#include "Client.hpp"
#include "utils.hpp"
#include "NotFoundError.hpp"

Client::Client(KQueue &kq, int fd) : FdInterface(kq, kFdClient, fd)
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

int IsRequestEnd(const std::string &request_message)
{
  return (request_message.find(CRLF) != std::string::npos);
}

FdInterfaceType Client::ParseReq()
{
  if (!IsRequestEnd(request_message))
    return kFdNone;

  std::string tmp = request_message.substr(request_message.find(CRLF) + 4);
  request_message = request_message.substr(0, request_message.find(CRLF));
  std::cout << "request_message: " << request_message << std::endl;

  if (request == nullptr)
    request = new RequestHeader(request_message);
  else if(request->FindItem("Transfer-Encoding")->first != ""
    && request->FindItem("Transfer-Encoding")->second->value == "chunked")
    // chunked
    // TODO : RequestHeader class need parse chunked body
    request->SetBody(request_message);
  else
    request->SetBody(request_message);

  request_message = tmp;

  if (request->FindItem("Content-Length")->first != ""
    && atoi(request->GetItem("Content-Length").value.c_str()) > 0
    && request->body.size() <= 0)
    return kFdClient;
  else if(request_message.find("cgi") != std::string::npos) { // TODO : CGI Check
    return kFdCgi;
  }
  else if(request != nullptr) {
    return kFdFileio;
  }
  else
    return kFdNone;
}

const std::string Client::GetFilePath() const
{
  std::string path = "." + request->host;
  //std::string path("html/index.html");
  return path;
}
