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
      break;
    case kFdPutMethod:
      new PutMethod(client->kq, client->GetFilePath(), client);
      client->request = nullptr;
      client->response = nullptr;
      break;
    case kFdPostMethod:
      new PostMethod(client->kq, client->GetFilePath(), client);
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

void PutMethod_Event_Read(PutMethod *putmethod)
{
  if (putmethod->EventRead() <= 0)
  {
    putmethod->SetResponseMessage();
    putmethod->kq.DeleteEvent(putmethod->interface_fd, EVFILT_READ);
    putmethod->kq.AddEvent(putmethod->target_fd, EVFILT_WRITE, putmethod);
  }
}

void PutMethod_Event_Write(PutMethod *putmethod, int fd)
{
  if (fd == putmethod->target_fd)
  {
    if (putmethod->EventWrite() <= 0) {
      putmethod->kq.DeleteEvent(putmethod->target_fd, EVFILT_WRITE);
      if (putmethod->request->GetItem("Connection").value == "close")
        delete putmethod->client;
      delete putmethod;
    }
  }
  else
  {
    if (putmethod->FileWrite() <= 0) {
      putmethod->SetResponseMessage();
      putmethod->kq.DeleteEvent(putmethod->interface_fd, EVFILT_WRITE);
      putmethod->kq.AddEvent(putmethod->target_fd, EVFILT_WRITE, putmethod);
    }
  }
  return ;
}

void PostMethod_Event_Read(PostMethod *postmethod)
{
  if (postmethod->EventRead() <= 0)
  {
    postmethod->SetResponseMessage();
    postmethod->kq.DeleteEvent(postmethod->interface_fd, EVFILT_READ);
    postmethod->kq.AddEvent(postmethod->target_fd, EVFILT_WRITE, postmethod);
  }
}

void PostMethod_Event_Write(PostMethod *postmethod, int fd)
{
  if (fd == postmethod->target_fd)
  {
    if (postmethod->EventWrite() <= 0) {
      postmethod->kq.DeleteEvent(postmethod->target_fd, EVFILT_WRITE);
      if (postmethod->request->GetItem("Connection").value == "close")
        delete postmethod->client;
      delete postmethod;
    }
  }
  else
  {
    if (postmethod->FileWrite() <= 0) {
      postmethod->SetResponseMessage();
      postmethod->kq.DeleteEvent(postmethod->interface_fd, EVFILT_WRITE);
      postmethod->kq.AddEvent(postmethod->target_fd, EVFILT_WRITE, postmethod);
    }
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
    case kFdPutMethod:
      PutMethod_Event_Read(static_cast<PutMethod *>(target));
      break;
    case kFdPostMethod:
      PostMethod_Event_Read(static_cast<PostMethod *>(target));
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
    case kFdPutMethod:
      PutMethod_Event_Write(static_cast<PutMethod *>(target), event.ident);
      break;
    case kFdPostMethod:
      PostMethod_Event_Write(static_cast<PostMethod *>(target), event.ident);
      break;
    default:
      break;
    }
  }
}
