#include "GetMethod.hpp"
#include "utils.hpp"

GetMethod::GetMethod(KQueue &kq, const std::string &path, Client *client) : Method(kq, client, kFdGetMethod)
{
  std::cout << "file: " << path << std::endl;
  try {
    interface_fd = open(path.c_str(), O_RDONLY);
    if (interface_fd < 0)
      throw NotFoundError();
  }
  catch (NotFoundError &e) {
    response->SetItem("Status", StatusCode(404));
  }
  if (response->status_code != "") {
    if (location->error_page[ft_stoi(response->status_code)] != "")
      interface_fd = open(location->error_page[ft_stoi(response->status_code)].c_str(), O_RDONLY);
    else {
      data = DefaultErrorPage(ft_stoi(response->status_code));
      SetResponseMessage();
      kq.AddEvent(target_fd, EVFILT_WRITE, this);
    }
  }
  fcntl(interface_fd, F_SETFL, O_NONBLOCK);
  kq.AddEvent(interface_fd, EVFILT_READ, this);
}

GetMethod::~GetMethod()
{
}
