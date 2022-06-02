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
    client->kq.DeleteEvent(client->interface_fd, EVFILT_READ);
    delete client;
  }
  switch(client->ParseReq())
  {
    case kFdClient:
      client->SetResponseMessage();
      client->kq.AddEvent(client->interface_fd, EVFILT_WRITE, client);
      break;
    case kFdGetMethod:
      new GetMethod(client->kq, client->GetFilePath(), client);
      client->request = nullptr;
      client->response = nullptr;
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
  {
    client->kq.DeleteEvent(client->interface_fd, EVFILT_WRITE);
    if(client->request->GetItem("Connection").value == "close")
      delete client;
  }
  return ;
}

void GetMethod_Event_Read(GetMethod *getmethod)
{
  if (getmethod->EventRead() <= 0)
  {
    getmethod->SetResponseMessage();
    getmethod->kq.AddEvent(getmethod->target_fd, EVFILT_WRITE, getmethod);
    getmethod->kq.DeleteEvent(getmethod->interface_fd, EVFILT_READ);
  }
}

void GetMethod_Event_Write(GetMethod *getmethod)
{
  if ( getmethod->EventWrite() <= 0) {
    getmethod->kq.DeleteEvent(getmethod->target_fd, EVFILT_WRITE);
    if (getmethod->request->GetItem("Connection").value == "close")
      delete getmethod->client;
    delete getmethod;
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
    case kFdGetMethod:
      GetMethod_Event_Read(static_cast<GetMethod *>(target));
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
    case kFdGetMethod:
      GetMethod_Event_Write(static_cast<GetMethod *>(target));
      break;
    default:
      break;
    }
  }
}
