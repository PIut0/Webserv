#include "Cgi.hpp"

Cgi::Cgi(KQueue &kq, const std::string &path, Client *client) : Method(kq, client, kFdCgi), cgi_write_idx(0)
{
  if (ft_stoi(response->status_code) >= 400) {
    ResponseErrorPage();
    return ;
  }

  std::string cgi_path, extension;
  cgi_read_data.reserve(this->request->body.size());

  try {
    pipe(this->fromCgi);
    pipe(this->toCgi);

    extension = this->request->host.substr(this->request->host.find_last_of('.'));
    cgi_path = this->location->cgi_info[extension];
    if (access(cgi_path.c_str(), X_OK) != 0){
      throw ForbiddenError();
    }

    pid_t pid = fork();

    fcntl(toCgi[FD_READ], F_SETFL, O_NONBLOCK);
    fcntl(toCgi[FD_WRITE], F_SETFL, O_NONBLOCK);
    fcntl(fromCgi[FD_READ], F_SETFL, O_NONBLOCK);
    fcntl(fromCgi[FD_WRITE], F_SETFD, O_NONBLOCK);

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
      if (execve(cgi_path.c_str(), argv, env) == -1)
        throw FdDupFailed();
    } else {
      close(fromCgi[FD_WRITE]);
      close(toCgi[FD_READ]);

      if (this->request->body.size() > 0 && this->request->method != HTTP_GET) {
        kq.AddEvent(toCgi[FD_WRITE], EVFILT_WRITE, this);
      }
      else {
        close(toCgi[FD_WRITE]);
      }
      kq.AddEvent(fromCgi[FD_READ], EVFILT_READ, this);
    }
  }
  catch (FdDupFailed &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  } catch (ForbiddenError &e) {
    SetResponseStatus(response, 403);
  }
  if (ft_stoi(response->status_code) >= 400) {
    ResponseErrorPage();
    return ;
  }
}

int Cgi::EventReadToCgi()
{
  char buf[BUFFER_SIZE];
  memset(buf, 0, BUFFER_SIZE);
  int n = read(fromCgi[FD_READ], buf, BUFFER_SIZE - 1);
  if (n <= 0)	// n == 0: 클라이언트에서 close & n == -1: 클라이언트 프로세스가 종료됨
    return n;
  buf[n] = '\0';
  cgi_read_data += buf;

  return n;
}

int Cgi::EventWriteToCgi()
{
  if (cgi_write_idx == 0) {
    cgi_write_data_size = request->body.size();
  }

  int len = cgi_write_data_size - cgi_write_idx > 65535 ? 65535 : cgi_write_data_size - cgi_write_idx;
  int n = write(toCgi[FD_WRITE], request->body.c_str() + cgi_write_idx, len);

  if (n <= 0)
    return n;

  cgi_write_idx += n;
  return cgi_write_data_size - cgi_write_idx;
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
    response->Parse(cgi_read_data);
  } catch (HttpParseInvalidResponse &e) {
    response->SetBody("");
  }
  SetResponseMessage();
}
