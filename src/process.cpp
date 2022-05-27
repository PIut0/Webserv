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
    close(client->interface_fd);
    delete client;
  }
  switch(client->ParseReq())
  {
    case kFdClient:
      client->kq.AddEvent(client->interface_fd, EVFILT_WRITE, client);
      break;
    case kFdFileio:
      new Fileio(client->kq, client->OpenFile(), *client);
      break;
    case kFdCgi:
      break;
    default:
      break;
  }
  return ;
}

void Client_Event_Write(Client *client)
{
  if (client->EventWrite() <= 0)
    client->kq.DeleteEvent(client->interface_fd, EVFILT_WRITE);
  return ;
}

void Fileio_Event_Read(Fileio *fileio)
{
  if (fileio->EventRead() <= 0)
  {
    fileio->kq.AddEvent(fileio->client.interface_fd, EVFILT_WRITE, fileio);
  }
}

void Fileio_Event_Write(Fileio *fileio)
{
  if (fileio->EventWrite() <= 0) {
    fileio->kq.DeleteEvent(fileio->interface_fd, EVFILT_WRITE);
    close(fileio->interface_fd);
    delete fileio;
  }
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
    case kFdFileio:
      Fileio_Event_Read(static_cast<Fileio *>(target));
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
    case kFdFileio:
      Fileio_Event_Write(static_cast<Fileio *>(target));
      break;
    default:
      break;
    }
  }
}
