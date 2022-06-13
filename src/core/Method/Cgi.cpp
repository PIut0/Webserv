#include "Method.hpp"

void CgiMethod(Method* method)
{
  std::string cgi_path, extension;
  method->read_data.reserve(method->client.request.body.size());

  pipe(method->fromCgi);
  pipe(method->toCgi);

  extension = method->client.request.host.substr(method->client.request.host.find_last_of('.'));
  cgi_path = method->location->cgi_info[extension];
  if (access(cgi_path.c_str(), X_OK) != 0){
    throw HTTP_STATUS_FORBIDDEN;
  }

  pid_t pid = fork();

  if (pid == PS_CHILD) {
    char **env = method->client.request.ToCgi(method->target_path);

    dup2(method->fromCgi[FD_WRITE], STDOUT_FILENO);
    dup2(method->toCgi[FD_READ], STDIN_FILENO);

    CloseFd(method->fromCgi[FD_READ]);
    CloseFd(method->fromCgi[FD_WRITE]);
    CloseFd(method->toCgi[FD_WRITE]);
    CloseFd(method->toCgi[FD_READ]);

    // print env
    for (int i = 0; env[i] != NULL; i++) {
      std::cout <<  env[i] << std::endl;
    }

    char *argv[2] = {const_cast<char *>(cgi_path.c_str()), 0};
    execve(cgi_path.c_str(), argv, env);

  } else {
    CloseFd(method->fromCgi[FD_WRITE]);
    CloseFd(method->toCgi[FD_READ]);

    fcntl(method->toCgi[FD_WRITE], F_SETFL, O_NONBLOCK);
    fcntl(method->fromCgi[FD_READ], F_SETFL, O_NONBLOCK);

    if (method->client.request.body.size() > 0 && method->client.request.method != HTTP_GET) {
      method->kq->AddEvent(method->toCgi[FD_WRITE], EVFILT_WRITE, method);
    }
    else {
      CloseFd(method->toCgi[FD_WRITE]);
    }
    method->kq->AddEvent(method->fromCgi[FD_READ], EVFILT_READ, method);
  }
}

int Method::EventReadToCgi()
{
  char buf[BUFFER_SIZE + 1];
  memset(buf, 0, BUFFER_SIZE + 1);
  int n = read(fromCgi[FD_READ], buf, BUFFER_SIZE);
  if (n <= 0)	// n == 0: 클라이언트에서 close & n == -1: 클라이언트 프로세스가 종료됨
    return n;
  buf[n] = '\0';
  read_data += buf;

  return n;
}

int Method::EventWriteToCgi()
{
  if (cgi_write_data_idx == 0) {
    cgi_write_data_size = client.request.body.size();
  }

  int len = cgi_write_data_size - cgi_write_data_idx > BUFFER_SIZE ? BUFFER_SIZE : cgi_write_data_size - cgi_write_data_idx;
  int n = write(toCgi[FD_WRITE], client.request.body.c_str() + cgi_write_data_idx, len);

  if (n <= 0)
    return n;

  cgi_write_data_idx += n;
  return cgi_write_data_size - cgi_write_data_idx;
}

void Method::SetResponseMessageCgi()
{
  try {
    client.response.Parse(read_data);
  } catch (HttpParseInvalidResponse &e) {
    client.response.SetBody("");
  }
  SetResponseMessage();
}
