#include "Method.hpp"

std::vector<std::string> GetFileList(const std::string &path)
{
  std::vector<std::string> res;
  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir(path.c_str())) == NULL)
    throw HTTP_STATUS_NOT_FOUND;

  while ((ent = readdir(dir)) != NULL) {
    std::string name(ent->d_name);
    if (ent->d_type == DT_DIR)
      name += '/';
    res.push_back(name);
  }
  closedir(dir);
  return res;
}

void GetMethod(Method* method)
{
  std::string path = method->target_path;

  if (access(path.c_str(), F_OK) != 0)
    throw HTTP_STATUS_NOT_FOUND;

  if (!IsRegularFile(path) && !IsDir(path))
    path += "/";

  if (IsDir(path)) {
    std::vector<std::string> file_list = GetFileList(path);

    for(size_t i=0; i<method->location->index.size(); i++) {
      for (size_t j=0; j<file_list.size(); j++) {
        if (file_list[j] == method->location->index[i]) {
          path += file_list[j];
          break;
        }
      }
      if (!IsDir(path))
        break;
    }

    if (IsDir(path)) {
      if (method->location->auto_index) {
        method->read_data = GetAutoindexPage(path, file_list);
        method->client.response.SetItem("Content-Type", "text/html");
        method->SetResponseMessage();
        method->kq->AddEvent(method->target_fd, EVFILT_WRITE, method);
        return ;
      }
      throw HTTP_STATUS_NOT_FOUND;
    }
  }

  if (access(path.c_str(), R_OK) != 0)
    throw HTTP_STATUS_FORBIDDEN;

  method->client.request.SetHost(path);
  method->interface_fd = open(path.c_str(), O_RDONLY);

  if (method->interface_fd == -1)
    throw HTTP_STATUS_INTERNAL_SERVER_ERROR;

  fcntl(method->interface_fd, F_SETFL, O_NONBLOCK);

  int is_eof = IsEOF(method->interface_fd);

  if (!is_eof) {
    method->SetResponseMessage();
    method->kq->AddEvent(method->target_fd, EVFILT_WRITE, method);
    return ;
  }
  else if (is_eof < 0)
    throw HTTP_STATUS_INTERNAL_SERVER_ERROR;

  method->kq->AddEvent(method->interface_fd, EVFILT_READ, method);
}
