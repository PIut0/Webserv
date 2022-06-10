#include "Cgi.hpp"

Cgi::Cgi(KQueue &kq, const std::string &path, Client *client) : Method(kq, client, kFdCgi)
{
  cgi_data = WSV_STR_EMPTY;
  std::string cgi_path, extension;

  try {
    pipe(this->fromCgi);
    pipe(this->toCgi);

    pid_t pid = fork();
    extension = this->request->host.substr(this->request->host.find_last_of('.'));
    cgi_path = this->location->cgi_info[extension];


    if (pid == PS_CHILD) {
      char **env = this->request->ToCgi(path);
      if (dup2(fromCgi[FD_WRITE], STDOUT_FILENO) == -1)
        throw FdDupFailed();

      if (dup2(toCgi[FD_READ], STDIN_FILENO) == -1)
        throw FdDupFailed();

      close(fromCgi[FD_READ]);
      close(toCgi[FD_WRITE]);
      close(toCgi[FD_READ]);
      char *argv[2] = {const_cast<char *>(cgi_path.c_str()), 0};
      //char **env = this->request->ToCgi(path);
      // TODO cgi_tester 경로 지정하기
      if (execve(cgi_path.c_str(), argv, env) == -1)
        throw FdDupFailed();
    } else {

      //for(int i=0;env[i];i++)
      //  std::cout << "env[" << i << "] = " << env[i] << std::endl;

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

  std::cout << "toCgi fd: " << toCgi[FD_WRITE] << std::endl;
  int len = cgi_data.size() > 65536 ? 65536 : cgi_data.size();
  int n = write(toCgi[FD_WRITE], cgi_data.c_str(), len);
  std::cout << "n: " << n << std::endl;

  if (n <= 0) {
    cgi_data = WSV_STR_EMPTY;
    return n;
  }
  cgi_data = cgi_data.substr(n);
  std::cout << "cgi_data: " << cgi_data.size() << std::endl;
  return cgi_data.size();
}

Cgi::~Cgi()
{
  close(fromCgi[FD_READ]);
  close(fromCgi[FD_WRITE]);
  close(toCgi[FD_READ]);
  close(toCgi[FD_WRITE]);
  if (interface_fd > 2)
    close(interface_fd);
}

void Cgi::SetResponseMessageCgi()
{
  try {
    response->Parse(cgi_data);
  } catch (HttpParseInvalidResponse &e) {
    response->SetBody("");
  }
  SetResponseMessage();
}


//toCgi fd: 9
//n: 65536
//cgi_data: 99934464
//toCgi fd: 9
//n: 65536
//cgi_data: 99868928
