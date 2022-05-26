#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "KQueue.hpp"
#include "Socket.hpp"
#include "utils.hpp"

class KQueue;

class Client : public Socket
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
