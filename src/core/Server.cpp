#include "Server.hpp"
#include "utils.hpp"

Server::Server(KQueue &kq, ServerBlock &_sb) : FdInterface(kq, kFdServer), server_block(_sb)
{
  if ((interface_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    ExitWithMsg("socket");
  int option = 1;
  setsockopt(interface_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(_sb.port);

  if (bind(interface_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    ExitWithMsg("bind");

  if (listen(interface_fd, 2048) == -1)
    ExitWithMsg("listen");

  fcntl(interface_fd, F_SETFL, O_NONBLOCK);
  kq.AddServer(*this);
}

Server::~Server()
{
  if (interface_fd > 2)
    close(interface_fd);
}

int Server::EventRead()
{
  int client_interface_fd;
  if ((client_interface_fd = accept(interface_fd, NULL, NULL)) == -1)
    ExitWithMsg("server_event_read");
  fcntl(client_interface_fd, F_SETFL, O_NONBLOCK);
  new Client(kq, client_interface_fd, this);

  return 1;
}

int Server::EventWrite()
{
  return 1;
}
