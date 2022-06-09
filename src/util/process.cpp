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
  if (client->EventRead() <= 0) {
    delete client;
    return;
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
    case kFdDeleteMethod:
      new DeleteMethod(client->kq, client->GetFilePath(), client);
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
}

void DeleteMethod_Event_Read(DeleteMethod *deletemethod)
{
  if (deletemethod->EventRead() <= 0)
  {
    deletemethod->SetResponseMessage();
    deletemethod->kq.DeleteEvent(deletemethod->interface_fd, EVFILT_READ);
    deletemethod->kq.AddEvent(deletemethod->target_fd, EVFILT_WRITE, deletemethod);
  }
}

void DeleteMethod_Event_Write(DeleteMethod *deletemethod)
{
  if (deletemethod->EventWrite() <= 0) {
    deletemethod->kq.DeleteEvent(deletemethod->target_fd, EVFILT_WRITE);
    if (deletemethod->request->GetItem("Connection").value == "close")
      delete deletemethod->client;
    delete deletemethod;
  }
}

void Cgi_Event_Read(Cgi *cgi, int ident)
{
  if (ident == cgi->interface_fd) {
    if (cgi->EventRead() <= 0)
    {
      cgi->SetResponseMessage();
      cgi->kq.DeleteEvent(cgi->interface_fd, EVFILT_READ);
      cgi->kq.AddEvent(cgi->target_fd, EVFILT_WRITE, cgi);
    }
  } else {
    if (cgi->EventReadToCgi() <= 0) { // 2
      cgi->SetResponseMessageCgi();
      cgi->kq.DeleteEvent(cgi->fromCgi[FD_READ], EVFILT_READ);
      close(cgi->fromCgi[FD_READ]);
      // target_fd로 쓰는 이벤트 등록하기
      cgi->kq.AddEvent(cgi->target_fd, EVFILT_WRITE, cgi);
    }
  }
}

void Cgi_Event_Write(Cgi *cgi, int ident)
{
  if (ident == cgi->target_fd) { // cgi response to client 3
    if (cgi->EventWrite() <= 0) {
      // client에 다 보냈으면 쓰는 이벤트 지우기
      cgi->kq.DeleteEvent(cgi->target_fd, EVFILT_WRITE);
      //if (cgi->request->GetItem("Connection").value == "close")
      // TODO : 이거 왜 안되는지 모르겠다.
        delete cgi->client;
      delete cgi;
    }
  } else {
    if (cgi->EventWriteToCgi() <= 0) { // request to cgi process 1
      // cgi에 다 썼으면 쓰는 이벤트 지우기
      cgi->kq.DeleteEvent(cgi->toCgi[FD_WRITE], EVFILT_WRITE);
      cgi->kq.AddEvent(cgi->fromCgi[FD_READ], EVFILT_READ, cgi);
      close(cgi->toCgi[FD_WRITE]);
      // 읽는 이벤트 등록하기 => 근데 이건 cgi.cpp에서 동시에 진행함
      // cgi->kq.AddEvent(cgi->fromCgi[FD_READ], EVFILT_READ, cgi);
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
    case kFdPutMethod:
      PutMethod_Event_Read(static_cast<PutMethod *>(target));
      break;
    case kFdPostMethod:
      PostMethod_Event_Read(static_cast<PostMethod *>(target));
      break;
    case kFdDeleteMethod:
      DeleteMethod_Event_Read(static_cast<DeleteMethod *>(target));
      break;
    case kFdCgi:
      Cgi_Event_Read(static_cast<Cgi *>(target), event.ident);
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
    case kFdDeleteMethod:
      DeleteMethod_Event_Write(static_cast<DeleteMethod *>(target));
      break;
    case kFdCgi:
      Cgi_Event_Write(static_cast<Cgi *>(target), event.ident);
    default:
      break;
    }
  }
  //if (event.flags & EV_EOF)
  //  std::cout << "EV_EOF" << std::endl;
}
