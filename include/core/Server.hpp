#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>

#include "FdInterface.hpp"
#include "KQueue.hpp"
//#include "Client.hpp"
// #include "utils.hpp"
#include "ServerBlock.hpp"

class KQueue;

class Server : public FdInterface
{
 private:
  Server();
 public:
  Server &operator=(const Server &other);
  Server(const Server &other);
  Server(KQueue *kq, ServerBlock &_sb);
  ~Server();

  int EventRead();
  int EventWrite();

  struct sockaddr_in  server_addr;
  ServerBlock         &server_block;
};

#endif
