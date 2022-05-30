#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "KQueue.hpp"
#include "FdInterface.hpp"
#include "utils.hpp"

class KQueue;

class Client : public FdInterface
{
 public:
  Client(KQueue &kq, int fd);
  ~Client();

  int EventRead();
  int EventWrite();
  int OpenFile();

  FdInterfaceType ParseReq();

  std::string req;
  std::string res;
  int has_body;
};

#endif
