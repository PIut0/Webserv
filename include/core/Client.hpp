#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "KQueue.hpp"
#include "FdInterface.hpp"
#include "utils.hpp"
#include "RequestHeader.hpp"

class KQueue;

class Client : public FdInterface
{
 public:
  Client(KQueue &kq, int fd);
  ~Client();

  int EventRead();
  int EventWrite();

  FdInterfaceType ParseReq();
  int OpenFile();

  RequestHeader request;
  std::string request_message;
  std::string response_message;
};

#endif
