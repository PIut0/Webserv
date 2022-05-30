#include "Fileio.hpp"
#include "utils.hpp"

Fileio::Fileio(KQueue &kq, int fd, Client *_client) : FdInterface(kq, kFdFileio, fd), data(""), client(_client)
{
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
  int n = write(client->interface_fd, data.c_str(), data.size());
  if (n <= 0)
    return n;
  data = data.substr(n);
  n = data.size();

  return n;
}
