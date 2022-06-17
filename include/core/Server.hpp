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
 public:
  Server &operator=(const Server &rv);
  Server(const Server &origin);
  Server(KQueue *kq, ServerBlock &sb);
  ~Server();

  int EventRead();
  int EventWrite();

  struct sockaddr_in  server_addr;
  ServerBlock         &server_block;

 private:
  Server();
};

#endif
