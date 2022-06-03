#include "GetMethod.hpp"
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
  //response->SetItem("Content-Type", "text/html");
}

std::vector<std::string> GetFileList(const std::string &path)
{
  std::vector<std::string> res;
  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir(path.c_str())) == NULL)
    throw NotFoundError();

  while ((ent = readdir(dir)) != NULL)
  {
    std::string name(ent->d_name);
    if (ent->d_type == DT_DIR)
      name += '/';
    res.push_back(name);
  }
  closedir(dir);
  return res;
}

GetMethod::GetMethod(KQueue &kq, const std::string &path, Client *client) : Method(kq, client, kFdGetMethod)
{
  std::cout << "file: " << path << std::endl;
  target_path = path;

  try {
    if (access(target_path.c_str(), F_OK) != 0)
      throw NotFoundError();

    if (IsDir(target_path)) {
      file_list = GetFileList(target_path);

      for(size_t i=0; i<location->index.size(); i++) {
        for (size_t j=0; j<file_list.size(); j++) {
          if (file_list[j] == location->index[i]) {
            target_path += file_list[j];
            break;
          }
        }
        if (!IsDir(target_path))
          break;
      }

      if (IsDir(target_path)) {
        if (location->auto_index) {
          data = GetAutoindexPage(target_path, file_list);
          response->SetItem("Content-Type", "text/html");
          SetResponseMessage();
          kq.AddEvent(target_fd, EVFILT_WRITE, this);
          return ;
        }
        else
          throw NotFoundError();
      }
    }

    if (access(target_path.c_str(), R_OK) != 0)
      throw ForbiddenError();

    request->SetHost(target_path);
    interface_fd = open(request->host.c_str(), O_RDONLY);
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
  kq.AddEvent(interface_fd, EVFILT_READ, this);
}

GetMethod::~GetMethod()
{
}
