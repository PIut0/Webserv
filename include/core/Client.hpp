#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "KQueue.hpp"
#include "FdInterface.hpp"
#include "utils.hpp"
#include "RequestHeader.hpp"
#include "Server.hpp"

class KQueue;

class Client : public FdInterface
{
 private:
  Client();
  Client(const Client &);
  Client &operator=(const Client &);
 public:
  Client(KQueue &kq, int fd, Server *server);
  ~Client();

  int EventRead();
  int EventWrite();

  int CheckCgi();
  FdInterfaceType ParseHeader(std::string &request_message);
  FdInterfaceType ParseBody(std::string &request_message);
  FdInterfaceType ParseReq();
  const std::string GetFilePath() const;

  Server *server;
  RequestHeader *request;
  std::string request_message;
  std::string response_message;
};

#endif
