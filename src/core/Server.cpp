#include "Server.hpp"
#include "utils.hpp"

Server &Server::operator=(const Server &rv)
{
  this->server_addr = rv.server_addr;
  return (*this);
}

Server::Server(const Server &origin) : FdInterface(origin.kq, kFdServer), server_block(origin.server_block)
{
  *this = origin;
}

Server::Server(KQueue *kq, ServerBlock &sb) : FdInterface(kq, kFdServer), server_block(sb)
{
  if ((interface_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    ThrowException("socket");
  int option = 1;
  setsockopt(interface_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(sb.port);

  if (bind(interface_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    ThrowException("bind");

  if (listen(interface_fd, 2048) == -1)
    ThrowException("listen");

  fcntl(interface_fd, F_SETFL, O_NONBLOCK);
  kq->AddServer(*this);
}

Server::~Server()
{
  kq->servers.erase(this);
  CloseFd(interface_fd);
  throw("Server Deleted");
}

int Server::EventRead()
{
  int client_interface_fd;
  if ((client_interface_fd = accept(interface_fd, NULL, NULL)) == -1)
    ThrowException("server_event_read");
  fcntl(client_interface_fd, F_SETFL, O_NONBLOCK);
  kq->AddClient(client_interface_fd, this);

  return 1;
}

int Server::EventWrite()
{
  return 1;
}
