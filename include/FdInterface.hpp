#ifndef FDINTERFACE_HPP
#define FDINTERFACE_HPP

#include "KQueue.hpp"
#include "utils.hpp"

class KQueue;

class FdInterface
{
 public:
  FdInterface(KQueue &kq) : kq(kq) {};
  FdInterface(KQueue &kq, int fd) : kq(kq), socket_fd(fd) {};
  virtual ~FdInterface() {};

  KQueue  &kq;
  int     socket_fd;

  virtual int EventRead() = 0;
  virtual int EventWrite() = 0;
};

#endif
