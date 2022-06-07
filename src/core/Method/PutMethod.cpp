#include "PutMethod.hpp"
#include "utils.hpp"

PutMethod::PutMethod(KQueue &kq, const std::string &path, Client *client) : Method(kq, client, kFdPutMethod)
{
  target_path = path;

  try {
    if (IsDir(target_path))
      throw NotFoundError(); // TODO : 400 BadRequest

    if (access(target_path.c_str(), F_OK) != 0 &&
        access(target_path.substr(0, target_path.find_last_of("/")).c_str(), W_OK) != 0)
      throw ForbiddenError();

    else if (access(target_path.c_str(), F_OK) == 0 && access(target_path.c_str(), W_OK) != 0)
      throw ForbiddenError();

    request->SetHost(target_path);
    interface_fd = open(request->host.c_str(), O_CREAT | O_WRONLY, 0644);
    if (interface_fd < 0)
      throw InternalServerError();

  } catch (NotFoundError &e) {
    SetResponseErrorPage(response, 404);
  } catch (ForbiddenError &e) {
    SetResponseErrorPage(response, 403);
  } catch (InternalServerError &e) {
    SetResponseErrorPage(response, 500);
  }

  if (response->status_code != "") {
    ResponseErrorPage();
    return ;
  }

  fcntl(interface_fd, F_SETFL, O_NONBLOCK);
  kq.AddEvent(interface_fd, EVFILT_WRITE, this);
}

int PutMethod::FileWrite()
{
  std::string res = request->body;
  int n = write(interface_fd, res.c_str(), res.size());
  if (n < 0)
    throw InternalServerError(); // TODO : Exception 처리 필요
  res = res.substr(n);
  n = res.size();

  return n;
}

PutMethod::~PutMethod()
{
}
