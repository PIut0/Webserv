#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "KQueue.hpp"
#include "Socket.hpp"
#include "utils.hpp"

#define CRLF "\r\n\r\n"

class KQueue;

class Client : public Socket
{
 public:
  std::string req;
  std::string res;
  int has_body;

  Client(KQueue &kq, int fd);
  ~Client();

  int EventRead();
  int EventWrite();

  void ParseReq();
};

#endif
