#include "Server.hpp"
#include "utils.hpp"

Server::Server(KQueue &kq, ServerBlock &_sb) : FdInterface(kq), server_block(_sb)
{
  if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    ExitWithPerror("socket");

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(_sb.port);

  if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    ExitWithPerror("bind");

  if (listen(socket_fd, 64) == -1)
    ExitWithPerror("listen");

  fcntl(socket_fd, F_SETFL, O_NONBLOCK);
}

Server::~Server()
{
  close(socket_fd);
}

int Server::EventRead()
{
  int client_socket_fd;
  if ((client_socket_fd = accept(socket_fd, NULL, NULL)) == -1)
    ExitWithPerror("server_event_read");
  new Client(kq, client_socket_fd);

  return 1;
}

int Server::EventWrite()
{
  return 1;
}
