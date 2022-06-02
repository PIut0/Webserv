#include "Fileio.hpp"
#include "utils.hpp"

Fileio::Fileio(KQueue &kq, const std::string &path, Client *client) : FdInterface(kq, kFdFileio), data("")
{
  std::cout << "file: " << path << std::endl;
  target_fd = client->interface_fd;
  request = client->request;
  response = client->response;
  location = client->GetLocationBlock();
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

Fileio::~Fileio()
{
  close(interface_fd);
}

int IsEOF(int fd)
{
  char buf[1];
  int n = read(fd, buf, sizeof(buf));
  if (n > 0)
    lseek(fd, -1, SEEK_CUR);
  return n;
}

int Fileio::EventRead()
{
  char buf[1024];
  int n = read(interface_fd, buf, sizeof(buf) - 1);
  buf[n] = '\0';
  data += buf;

  return IsEOF(interface_fd);
}

int Fileio::EventWrite()
{
  std::string res = response->ToString();
  int n = write(target_fd, res.c_str(), res.size());
  if (n <= 0)
    return n;
  res = res.substr(n);
  n = res.size();

  return n;
}

void Fileio::SetResponseMessage()
{
  if (response->status_code == "")
    response->SetItem("Status", StatusCode(200));
  response->SetBody(data);
  response->SetItem("Content-Length", ft_itos(response->body.size()));
  response->SetItem("Content-Type", "text/html");
  if (request->FindItem("Connection")->first == "Connection")
    response->SetItem("Connection", request->FindItem("Connection")->second->value);
  else
    response->SetItem("Connection", "keep-alive");
}
