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

#define PORT 8080

class Client;
class KQueue;
class ServerBlock;

class Fileio : public FdInterface
{
 public:
  Fileio(KQueue &kq, int fd, Client *_client);
  ~Fileio();

  int EventRead();
  int EventWrite();

  std::string data;
  Client *client;
};

#endif
