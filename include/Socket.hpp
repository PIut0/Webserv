#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "KQueue.hpp"
#include "utils.hpp"

class KQueue;

class Socket
{
 public:
  Socket(KQueue &kq) : kq(kq) {};
  Socket(KQueue &kq, int fd) : kq(kq), socket_fd(fd) {};
  virtual ~Socket() {};

  KQueue  &kq;
  int     socket_fd;

  virtual int EventRead() = 0;
  virtual int EventWrite() = 0;
};

#endif
