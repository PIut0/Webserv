#include "Client.hpp"
#include "utils.hpp"

Client::Client(KQueue &kq, int fd) : FdInterface(kq, kFdClient, fd), has_body(0)
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
  req += buf;

  return n;
}

int Client::EventWrite()
{
  int n = write(interface_fd, res.c_str(), res.size());
  if (n <= 0)
    return n;
  res = res.substr(n);
  n = res.size();

  return n;
}

int IsRequestEnd(const std::string &req)
{
  return (req.find(CRLF) != std::string::npos);
}

// TODO : parse http req
FdInterfaceType Client::ParseReq()
{
  if (!IsRequestEnd(req))
    return kFdNone;

  std::string tmp = req.substr(req.find(CRLF) + 4);
  req = req.substr(0, req.find(CRLF));

  // TODO : has body
  //has_body = (req.find("Content-Length") != std::string::npos && !has_body) ? 1 : 0;

  if(req.find("Fileio") != std::string::npos) {
    res = req;
    req = tmp;
    return kFdFileio;
  }
  else if(req.find("Cgi") != std::string::npos) {
    res = req;
    req = tmp;
    return kFdCgi;
  }
  else {
    res = req;
    req = tmp;
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
