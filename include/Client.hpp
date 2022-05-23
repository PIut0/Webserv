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
  Client(KQueue &kq, int fd);
  ~Client();

  std::string req;
  std::string res;
  int has_body;

  int EventRead();
  int EventWrite();
  void ParseReq();
};

#endif
