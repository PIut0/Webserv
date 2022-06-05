#include "Cgi.hpp"

Cgi::Cgi(KQueue &kq, const std::string &path, Client *client) : FdInterface(kq, kFdCgi), client(client)
{
  target_fd = client->interface_fd;

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

int Cgi::EventRead()
{

  return 0;
}

int Cgi::EventWrite()
{

  return 0;
}

int Cgi::EventWriteToCgi()
{

  return 0;
}

Cgi::~Cgi() {}
