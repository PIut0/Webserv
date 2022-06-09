#include "Cgi.hpp"

Cgi::Cgi(KQueue &kq, const std::string &path, Client *client) : Method(kq, client, kFdCgi)
{
  std::cout << "path: " << path << std::endl;
  cgi_data = WSV_STR_EMPTY;
  std::string cgi_path, extension;

  try {
    pipe(this->fromCgi);
    pipe(this->toCgi);

    pid_t pid = fork();
    extension = this->request->host.substr(this->request->host.find_last_of('.'));
    cgi_path = this->location->cgi_info[extension];

    if (pid == PS_CHILD) {
      if (dup2(fromCgi[FD_WRITE], STDOUT_FILENO) == -1)
        throw FdDupFailed();

      if (dup2(toCgi[FD_READ], STDIN_FILENO) == -1)
        throw FdDupFailed();

      close(fromCgi[FD_READ]);
      close(toCgi[FD_READ]);

      char **env = this->request->ToCgi(path);
      // TODO cgi_tester 경로 지정하기
      if (execve(cgi_path.c_str(), NULL, env) == -1)
        throw FdDupFailed();
    } else {
      close(fromCgi[FD_WRITE]);
      close(toCgi[FD_READ]);

      fcntl(toCgi[FD_WRITE], F_SETFL, O_NONBLOCK);
      fcntl(fromCgi[FD_READ], F_SETFL, O_NONBLOCK);

      if (this->request->body.size() > 0 && this->request->method != HTTP_GET) {
        kq.AddEvent(toCgi[FD_WRITE], EVFILT_WRITE, this);
      }
      else {
        close(toCgi[FD_WRITE]);
        kq.AddEvent(fromCgi[FD_READ], EVFILT_READ, this);
      }
    }
  }
  catch (FdDupFailed &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
}

int Cgi::EventReadToCgi()
{
  char buf[1024];
  int n = read(fromCgi[FD_READ], buf, sizeof(buf) - 1);
  if (n <= 0)	// n == 0: 클라이언트에서 close & n == -1: 클라이언트 프로세스가 종료됨
    return n;
  buf[n] = '\0';
  cgi_data += buf;

  return n;
}

int Cgi::EventWriteToCgi()
{
  if (cgi_data == WSV_STR_EMPTY) {
    cgi_data = request->body;
  }

  int n = write(toCgi[FD_WRITE], cgi_data.c_str(), cgi_data.size());
  if (n <= 0) {
    cgi_data = WSV_STR_EMPTY;
    return n;
  }
  cgi_data = cgi_data.substr(n);

  return cgi_data.size();
}

Cgi::~Cgi()
{
  close(fromCgi[FD_READ]);
  close(fromCgi[FD_WRITE]);
  close(toCgi[FD_READ]);
  close(toCgi[FD_WRITE]);
}

void Cgi::SetResponseMessageCgi()
{
  try {
    response->Parse(cgi_data);
  } catch (HttpParseInvalidResponse &e) {
    std::cerr << "HttpParseInvalidResponse" << std::endl;
    SetResponseStatus(response, 400);
  }
  std::cout << "response: " << response->ToString() << std::endl;
  //SetResponseMessage();
}
