#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "KQueue.hpp"
#include "FdInterface.hpp"
#include "utils.hpp"
#include "RequestHeader.hpp"
#include "ResponseHeader.hpp"
#include "Server.hpp"
#include "Method.hpp"

class Server;
class KQueue;
class Method;
class LocationBlock;

#define D_CRLF "\r\n\r\n"

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

  LocationBlock *GetLocationBlock();
  int CheckCgi();
  int CheckRequest();
  FdInterfaceType ParseHeader();
  FdInterfaceType ParseBody();
  FdInterfaceType ParseReq();
  const std::string GetFilePath();
  void SetResponseMessage();
  void SetSocketHitTime();

  Server *server;
  RequestHeader *request;
  ResponseHeader *response;
  std::set<Method *> method_list;
  std::string request_message;
};

#endif
