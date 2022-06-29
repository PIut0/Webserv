#include "Method.hpp"

void DeleteMethod(Method* method)
{
  std::string path = method->target_path;

  int is_file = access(path.c_str(), F_OK);

  if (is_file != 0)
    throw HTTP_STATUS_NOT_FOUND;

  if (access(path.substr(0, path.find_last_of("/")).c_str(), W_OK) != 0)
    throw HTTP_STATUS_FORBIDDEN;

  method->client.request.SetHost(path);

  if (remove(path.c_str()))
    throw HTTP_STATUS_INTERNAL_SERVER_ERROR;

  method->SetResponseMessage();
  method->kq->AddEvent(method->target_fd, EVFILT_WRITE, method);
}
