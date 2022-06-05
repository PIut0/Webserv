#include "Cgi.hpp"

// TODO CGI 클래스 새로 만들기
// TODO ToCgi에 path 넣어주기
// TODO ident 값으로 비교하기
// TODO signal issue 등록하기
Cgi::Cgi(KQueue &kq, Client *client, FdInterfaceType type, const std::string &path) : FdInterface(kq, type)
{
  pipe(this->fromCgi);
  pipe(this->toCgi);

  pid_t pid = fork();
  if (pid == PS_CHILD) {
    if (dup2(fromCgi[FD_WRITE], STDOUT_FILENO)== -1)
      throw FdDupFailed(); // TODO signal 처리

    if (dup2(toCgi[FD_READ], STDIN_FILENO) == -1)
      throw FdDupFailed();

    close(fromCgi[FD_READ]);
    close(toCgi[FD_READ]);
    char **env = client->request->ToCgi(CGI_PHP);
    if (execve("./cgi_tester", NULL, env) == -1)
      throw FdDupFailed();
  } else {
    close(fromCgi[FD_WRITE]);
    close(toCgi[FD_READ]);

    if (client->request->body.size() > 0) {
      fcntl(toCgi[FD_WRITE], F_SETFL, O_NONBLOCK);
      kq.AddEvent(toCgi[FD_WRITE], EVFILT_WRITE, this);
    }
    fcntl(fromCgi[FD_READ], F_SETFL, O_NONBLOCK);
    kq.AddEvent(fromCgi[FD_READ], EVFILT_READ, this);
  }
}

Cgi::~Cgi() {}
