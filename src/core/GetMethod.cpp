#include "GetMethod.hpp"
#include "utils.hpp"

int IsDir(const std::string &path)
{
  return (path.back() == '/');
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
          SetResponseMessage();
          kq.AddEvent(target_fd, EVFILT_WRITE, this);
          return ;
        }
        else
          throw NotFoundError();
      }
    }

    interface_fd = open(target_path.c_str(), O_RDONLY);
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
      return ;
    }
  }
  fcntl(interface_fd, F_SETFL, O_NONBLOCK);
  kq.AddEvent(interface_fd, EVFILT_READ, this);
}

GetMethod::~GetMethod()
{
}
