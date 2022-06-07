#include "PutMethod.hpp"
#include "utils.hpp"

int IsDir(const std::string &path)
{
  return (path.back() == '/');
}

void SetResponseErrorPage(ResponseHeader *response, const int code)
{
  if (!response)
    return ;
  response->SetItem("Status", StatusCode(code));
}

PutMethod::PutMethod(KQueue &kq, const std::string &path, Client *client) : Method(kq, client, kFdGetMethod)
{
  std::cout << "file: " << path << std::endl;
  target_path = path;

  try {
    if (IsDir(target_path))
      throw NotFoundError(); // 400 BadRequest

    if (access(target_path.c_str(), F_OK) != 0 &&
        access(target_path.substr(0, target_path.find_last_of("/")).c_str(), W_OK) != 0)
      throw ForbiddenError();

    else if (access(target_path.c_str(), F_OK) == 0 && access(target_path.c_str(), W_OK) != 0)
      throw ForbiddenError();

    request->SetHost(target_path);
    interface_fd = open(request->host.c_str(), O_WRONLY);
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
    if (location->error_page != "")
      interface_fd = open(location->error_page.c_str(), O_RDONLY);
    else {
      data = "";
      SetResponseMessage();
      kq.AddEvent(target_fd, EVFILT_WRITE, this);
      return ;
    }
  }
  fcntl(interface_fd, F_SETFL, O_NONBLOCK);
  kq.AddEvent(interface_fd, EVFILT_WRITE, this);
}

PutMethod::~PutMethod()
{
}
