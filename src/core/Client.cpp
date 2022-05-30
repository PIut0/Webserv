#include "Client.hpp"
#include "utils.hpp"

Client::Client(KQueue &kq, int fd) : FdInterface(kq, kFdClient, fd)
{
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

// TODO : parse http request_message
FdInterfaceType Client::ParseReq()
{
  if (!IsRequestEnd(request_message))
    return kFdNone;

  // TODO : has body
  //has_body = (request_message.find("Content-Length") != std::string::npos && !has_body) ? 1 : 0;

  std::string tmp = request_message.substr(request_message.find(CRLF) + 4);
  request_message = request_message.substr(0, request_message.find(CRLF));

  if(request_message.find("Fileio") != std::string::npos) {
    response_message = request_message;
    request_message = tmp;
    return kFdFileio;
  }
  else if(request_message.find("Cgi") != std::string::npos) {
    response_message = request_message;
    request_message = tmp;
    return kFdCgi;
  }
  else {
    response_message = request_message;
    request_message = tmp;
    return kFdClient;
  }
}

int Client::OpenFile()
{
  //std::string &path = request_path
  std::string path("html/index.html");
  int fd = open(path.c_str(), O_RDONLY);
  if (fd < 0)
    return fd;  // TODO : Error exception

  return fd;
}
