#include "process.hpp"

void Server_Event_Read(Server *server)
{
  server->EventRead();
}

void Server_Event_Write(Server *server)
{
  server->EventWrite();
}

void Client_Event_Read(Client *client)
{
  if (client->EventRead() <= 0)
  {
    std::cout << "fd: " << client->interface_fd << ": delete client" << std::endl;
    close(client->interface_fd);
    delete client;
  }
  return ;
}

void Client_Event_Write(Client *client)
{
  if (client->EventWrite() <= 0)
    client->kq.DeleteEvent(client->interface_fd, EVFILT_WRITE);
  return ;
}

void Process(FdInterface *target, struct kevent event)
{
  if (event.filter == EVFILT_READ)
  {
    switch (target->interface_type)
    {
    case kFdServer:
      Server_Event_Read(static_cast<Server *>(target));
      break;
    case kFdClient:
      Client_Event_Read(static_cast<Client *>(target));
      break;
    default:
      break;
    }
  }
  else if (event.filter == EVFILT_WRITE)
  {
    switch (target->interface_type)
    {
    case kFdServer:
      Server_Event_Write(static_cast<Server *>(target));
      break;
    case kFdClient:
      Client_Event_Write(static_cast<Client *>(target));
      break;
    default:
      break;
    }
  }
}
