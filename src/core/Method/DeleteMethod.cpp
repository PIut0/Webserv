#include "DeleteMethod.hpp"
#include "utils.hpp"

DeleteMethod::DeleteMethod(KQueue &kq, const std::string &path, Client *client) : Method(kq, client, kFdPostMethod)
{
  target_path = path;
  try {
    int is_file = access(target_path.c_str(), F_OK);

    if (is_file != 0)
      throw NotFoundError();

    if (access(target_path.substr(0, target_path.find_last_of("/")).c_str(), W_OK) != 0)
      throw ForbiddenError();

    request->SetHost(target_path);

    if (remove(target_path.c_str()))
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

  SetResponseMessage();
  kq.AddEvent(target_fd, EVFILT_WRITE, this);
}

DeleteMethod::~DeleteMethod()
{
}
