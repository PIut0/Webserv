#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "KQueue.hpp"
#include "utils.hpp"

class KQueue;

class Socket
{
public:
  KQueue &kq;
  int socket_fd;

  Socket(KQueue &_kq) : kq(_kq) {};
  Socket(KQueue &_kq, int fd) : kq(_kq), socket_fd(fd) {};
  virtual ~Socket() {};

  virtual int EventRead() = 0;
  virtual int EventWrite() = 0;
};

#endif
