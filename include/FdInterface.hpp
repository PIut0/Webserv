#ifndef FDINTERFACE_HPP
#define FDINTERFACE_HPP

#include "KQueue.hpp"
#include "utils.hpp"

class KQueue;

enum FdInterfaceType {
  kFdServer,
  kFdClient,
  kFdFileIO,
  kFdCgi
};

class FdInterface
{
 public:
  FdInterface(KQueue &kq, FdInterfaceType type) : kq(kq), interface_type(type) {};
  FdInterface(KQueue &kq, FdInterfaceType type, int fd) : kq(kq), interface_type(type), interface_fd(fd) {};
  virtual ~FdInterface() {};

  KQueue          &kq;
  FdInterfaceType interface_type;
  int             interface_fd;

  virtual int EventRead() = 0;
  virtual int EventWrite() = 0;
};

#endif
