#include "PostMethod.hpp"
#include "utils.hpp"

PostMethod::PostMethod(KQueue &kq, const std::string &path, Client* &client) : Method(kq, client, kFdPostMethod)
{
  if (ft_stoi(response->status_code) >= 400) {
    ResponseErrorPage();
    return ;
  }

  target_path = path;
  try {
    int is_file = access(target_path.c_str(), F_OK);
    if (IsDir(target_path))
      throw NotFoundError(); // TODO : 400 BadRequest

    if (is_file != 0 &&
        access(target_path.substr(0, target_path.find_last_of("/")).c_str(), W_OK) != 0)
      throw ForbiddenError();

    else if (is_file == 0 && access(target_path.c_str(), W_OK) != 0)
      throw ForbiddenError();

    request->SetHost(target_path);

    if (is_file != 0)
      SetResponseStatus(response, 201);

    interface_fd = open(request->host.c_str(), O_WRONLY | O_CREAT, 0666);

    if (interface_fd < 0)
      throw InternalServerError();

  } catch (NotFoundError &e) {
    SetResponseStatus(response, 404);
  } catch (ForbiddenError &e) {
    SetResponseStatus(response, 403);
  } catch (InternalServerError &e) {
    SetResponseStatus(response, 500);
  }

  if (ft_stoi(response->status_code) >= 400) {
    ResponseErrorPage();
    return ;
  }

  fcntl(interface_fd, F_SETFL, O_NONBLOCK);
  kq.AddEvent(interface_fd, EVFILT_WRITE, this);
}

int PostMethod::FileWrite()
{
  std::string res = request->body;
  int n = write(interface_fd, res.c_str(), res.size());
  if (n < 0)
    throw InternalServerError(); // TODO : Exception 처리 필요
  res = res.substr(n);
  n = res.size();

  return n;
}

PostMethod::~PostMethod()
{
}
