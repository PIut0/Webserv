#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>

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

class Fileio : public FdInterface
{
 private:
  Fileio();
  Fileio(const Fileio &);
  Fileio &operator=(const Fileio &);
 public:
  Fileio(KQueue &kq, const std::string &path, Client *client);
  ~Fileio();

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
