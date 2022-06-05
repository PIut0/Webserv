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
      client->kq.DisableEvent(client->interface_fd, EVFILT_READ, client);
      client->kq.AddEvent(client->interface_fd, EVFILT_WRITE, client);
      break;
    case kFdGetMethod:
      new GetMethod(client->kq, client->GetFilePath(), client);
      client->request = nullptr;
      client->response = nullptr;
      break;
    case kFdCgi:
      new Cgi(client->kq, client->GetFilePath(), client);
      client->request = nullptr;
      client->response = nullptr;
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
    client->kq.EnableEvent(client->interface_fd, EVFILT_READ, client);
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
    getmethod->kq.DeleteEvent(getmethod->interface_fd, EVFILT_READ);
    getmethod->kq.AddEvent(getmethod->target_fd, EVFILT_WRITE, getmethod);
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

void Cgi_Event_Read(Cgi *cgi)
{
  if (cgi->EventRead() <= 0)
  {
    cgi->SetResponseMessage();
    cgi->kq.DeleteEvent(cgi->fromCgi[FD_READ], EVFILT_READ);
    cgi->kq.AddEvent(cgi->target_fd, EVFILT_WRITE, cgi);
  }
}

void Cgi_Event_Write(Cgi *cgi, const uintptr_t &ident)
{
  if (ident == cgi->target_fd) {
    if (cgi->EventWrite() <= 0) {
      // cgi response to client
    }
  } else {
    if (cgi->EventWriteToCgi() <= 0) {
      // request to cgi process
    }
  }
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
    case kFdCgi:
      Cgi_Event_Read(static_cast<Cgi *>(target));
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
    case kFdCgi:
      Cgi_Event_Write(static_cast<Cgi *>(target), event.ident);
      break;
    default:
      break;
    }
  }
}
