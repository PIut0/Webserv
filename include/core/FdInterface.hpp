#ifndef FDINTERFACE_HPP
#define FDINTERFACE_HPP

#include "KQueue.hpp"
#include "System.hpp"
#include "utils.hpp"

class KQueue;

enum FdInterfaceType {
  kFdServer,
  kFdClient,
  kFdGetMethod,
  kFdPutMethod,
  kFdPostMethod,
  kFdDeleteMethod,
  kFdCgi,
  kFdNone
};

class FdInterface : public System
{
 public:
  FdInterface(KQueue &kq, FdInterfaceType type) : kq(kq), interface_type(type) {};
  FdInterface(KQueue &kq, FdInterfaceType type, int fd) : kq(kq), interface_type(type), interface_fd(fd) {};
  virtual ~FdInterface() {};

  virtual int EventRead() = 0;
  virtual int EventWrite() = 0;

  KQueue          &kq;
  FdInterfaceType interface_type;
  int             interface_fd;
};

#endif
