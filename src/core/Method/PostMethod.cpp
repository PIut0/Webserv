#include "Method.hpp"

void PostMethod(Method* method)
{
  std::string path = method->target_path;

  int is_file = access(path.c_str(), F_OK);
  if (IsDir(path))
    throw HTTP_STATUS_NOT_FOUND;

  if (is_file != 0 &&
      access(path.substr(0, path.find_last_of("/")).c_str(), W_OK) != 0) {
    throw HTTP_STATUS_FORBIDDEN;
  } else if (is_file == 0 && access(path.c_str(), W_OK) != 0) {
    throw HTTP_STATUS_FORBIDDEN;
  }

  method->client.request.SetHost(path);

  if (is_file != 0)
    method->SetResponseStatus(201);

  method->interface_fd = open(method->client.request.host.c_str(), O_WRONLY | O_CREAT, 0666);

  if (method->interface_fd < 0)
    throw HTTP_STATUS_INTERNAL_SERVER_ERROR;

  fcntl(method->interface_fd, F_SETFL, O_NONBLOCK);
  method->kq->AddEvent(method->interface_fd, EVFILT_WRITE, method);
}
