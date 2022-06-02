#ifndef METHOD_HPP
#define METHOD_HPP

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <vector>

#include "KQueue.hpp"
#include "FdInterface.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "ServerBlock.hpp"
#include "NotFoundError.hpp"

#define PORT 8080

class Client;
class KQueue;
class ServerBlock;

class Method : public FdInterface
{
 private:
  Method();
  Method(const Method &);
  Method &operator=(const Method &);
 public:
  Method(KQueue &kq, Client *client, FdInterfaceType type);
  virtual ~Method();

  int EventRead();
  int EventWrite();
  void SetResponseMessage();

  int target_fd;
  std::string data;
  RequestHeader *request;
  ResponseHeader *response;
  LocationBlock *location;
};

#endif
