#include "Cgi.hpp"

Cgi::Cgi(KQueue &kq, const std::string &path, Client *client) : Method(kq, client, kFdCgi)
{
  target_fd = client->interface_fd;
  request = (client->request) ? client->request : new RequestHeader();
  response = (client->response) ? client->response : new ResponseHeader();
  location = client->GetLocationBlock();
  data_ = WSV_STR_EMPTY;

  std::string cgi_path, extension;

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

    if (this->request->body.size() > 0) {
      fcntl(toCgi[FD_WRITE], F_SETFL, O_NONBLOCK);
      kq.AddEvent(toCgi[FD_WRITE], EVFILT_WRITE, this);
    }
    fcntl(fromCgi[FD_READ], F_SETFL, O_NONBLOCK);
    kq.AddEvent(fromCgi[FD_READ], EVFILT_READ, this);
  }
}

/**
 * @brief read cgi program output.
 *
 * fd = fromCgi[FD_READ];
 *
 * @return int result of read()
 */
int Cgi::EventRead()
{
  char buf[1024];
  int n = read(fromCgi[FD_READ], buf, sizeof(buf) - 1);
  if (n <= 0)	// n == 0: 클라이언트에서 close & n == -1: 클라이언트 프로세스가 종료됨
    return n;
  buf[n] = '\0';
  data_ += buf;

  return n;
}


/**
 * @brief write to cgi program
 *
 * fd = toCgi[FD_WRITE]
 * data = body
 * @return int result of write()
 */
int Cgi::EventWrite()
{
  if (data_ == WSV_STR_EMPTY) {
    data_ = request->body;
  }

  int n = write(interface_fd, data_.c_str(), data_.size());
  if (n <= 0) {
    data_ = WSV_STR_EMPTY;
    return n;
  }
  data_ = data_.substr(n);

  return data_.size();
}

/**
 * @brief write to client ( response 보내는 함수 ) - 3번
 *
 * fd = target_fd
 * data = response->ToString();
 * @return int result of write()
 */
int Cgi::EventWriteToCgi()
{
  if (data_ == WSV_STR_EMPTY) {
    data_ = response->ToString();
  }

  int n = write(target_fd, data_.c_str(), data_.size());
  if (n <= 0)
    return n;
  data_ = data_.substr(n);

  return data_.size();
}

// data_에 있는 내용을 토대로 responseHeader구성
void Cgi::SetResponseMessage()
{
  response->Parse(this->data_);
}

Cgi::~Cgi() {}
