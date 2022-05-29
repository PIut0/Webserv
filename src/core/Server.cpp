#include "Server.hpp"
#include "utils.hpp"

Server::Server(KQueue &kq, ServerBlock &_sb) : FdInterface(kq, kFdServer), server_block(_sb)
{
  if ((interface_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    ExitWithPerror("socket");

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(_sb.port);

  if (bind(interface_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    ExitWithPerror("bind");

  if (listen(interface_fd, 64) == -1)
    ExitWithPerror("listen");

  fcntl(interface_fd, F_SETFL, O_NONBLOCK);
}

Server::~Server()
{
  close(interface_fd);
}

int Server::EventRead()
{
  int client_interface_fd;
  if ((client_interface_fd = accept(interface_fd, NULL, NULL)) == -1)
    ExitWithPerror("server_event_read");
  new Client(kq, client_interface_fd);

  return 1;
}

int Server::EventWrite()
{
  return 1;
}