#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "KQueue.hpp"
#include "FdInterface.hpp"
#include "utils.hpp"

#define CRLF "\r\n\r\n"

class KQueue;

class Client : public FdInterface
{
 public:
  std::string req;
  std::string res;
  int has_body;

  Client(KQueue &kq, int fd);
  ~Client();

  int EventRead();
  int EventWrite();

  FdInterfaceType ParseReq();
  int OpenFile();
};

#endif
