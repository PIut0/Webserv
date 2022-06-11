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
  int status = client->EventRead();
  if (status <= 0) {
    // TODO : delete 부분과 밀접한 연관이 있음
    client->kq.delete_list.insert(client);
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
      client->method_list.insert(new GetMethod(client->kq, client->GetFilePath(), client));
      client->request = nullptr;
      client->response = nullptr;
      break;
    case kFdPutMethod:
      client->method_list.insert(new PutMethod(client->kq, client->GetFilePath(), client));
      client->request = nullptr;
      client->response = nullptr;
      break;
    case kFdPostMethod:
      client->method_list.insert(new PostMethod(client->kq, client->GetFilePath(), client));
      client->request = nullptr;
      client->response = nullptr;
      break;
    case kFdDeleteMethod:
      client->method_list.insert(new DeleteMethod(client->kq, client->GetFilePath(), client));
      client->request = nullptr;
      client->response = nullptr;
      break;
    case kFdCgi:
      client->method_list.insert(new Cgi(client->kq, client->GetFilePath(), client));
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
    client->kq.DisableEvent(client->interface_fd, EVFILT_WRITE, client);
    //client->kq.DeleteEvent(client->interface_fd, EVFILT_WRITE);

    if(client->request->GetItem("Connection").value == "close")
      //delete client;
      client->kq.delete_list.insert(client);
    //if (client->response) {
    //  delete client->response;
    //  client->response = nullptr;
    //}
    //if (client->request) {
    //  delete client->request;
    //  client->request = nullptr;
    //}
  }
}

void GetMethod_Event_Read(GetMethod *getmethod)
{
  if (getmethod->EventRead() <= 0)
  {
    getmethod->SetResponseMessage();
    getmethod->kq.DisableEvent(getmethod->interface_fd, EVFILT_READ, getmethod);
    //getmethod->kq.DeleteEvent(getmethod->interface_fd, EVFILT_READ);

    getmethod->kq.AddEvent(getmethod->target_fd, EVFILT_WRITE, getmethod);
  }
}

void GetMethod_Event_Write(GetMethod *getmethod)
{
  if (getmethod->EventWrite() <= 0) {
    //getmethod->kq.DisableEvent(getmethod->target_fd, EVFILT_WRITE, getmethod);
    getmethod->kq.DeleteEvent(getmethod->target_fd, EVFILT_WRITE);

    if (getmethod->request->GetItem("Connection").value == "close")
      getmethod->kq.delete_list.insert(getmethod->client);
    //delete getmethod;
    getmethod->kq.delete_list.insert(getmethod);
  }
}

void PutMethod_Event_Read(PutMethod *putmethod)
{
  if (putmethod->EventRead() <= 0)
  {
    putmethod->SetResponseMessage();
    //putmethod->kq.DisableEvent(putmethod->interface_fd, EVFILT_READ, putmethod);
    putmethod->kq.DeleteEvent(putmethod->interface_fd, EVFILT_READ);

    putmethod->kq.AddEvent(putmethod->target_fd, EVFILT_WRITE, putmethod);
  }
}

void PutMethod_Event_Write(PutMethod *putmethod, int fd)
{
  if (fd == putmethod->target_fd)
  {
    if (putmethod->EventWrite() <= 0) {
      putmethod->kq.DisableEvent(putmethod->target_fd, EVFILT_WRITE, putmethod);
      //putmethod->kq.DeleteEvent(putmethod->target_fd, EVFILT_WRITE);

      if (putmethod->request->GetItem("Connection").value == "close")
        //delete putmethod->client;
        putmethod->kq.delete_list.insert(putmethod->client);
      //delete putmethod;
      putmethod->kq.delete_list.insert(putmethod);
    }
  }
  else
  {
    if (putmethod->FileWrite() <= 0) {
      putmethod->SetResponseMessage();
      putmethod->kq.DisableEvent(putmethod->interface_fd, EVFILT_WRITE, putmethod);
      //putmethod->kq.DeleteEvent(putmethod->interface_fd, EVFILT_WRITE);

      putmethod->kq.AddEvent(putmethod->target_fd, EVFILT_WRITE, putmethod);
    }
  }
}

void PostMethod_Event_Read(PostMethod *postmethod)
{
  if (postmethod->EventRead() <= 0)
  {
    postmethod->SetResponseMessage();
    postmethod->kq.DisableEvent(postmethod->interface_fd, EVFILT_READ, postmethod);
    //postmethod->kq.DeleteEvent(postmethod->interface_fd, EVFILT_READ);

    postmethod->kq.AddEvent(postmethod->target_fd, EVFILT_WRITE, postmethod);
  }
}

void PostMethod_Event_Write(PostMethod *postmethod, int fd)
{
  if (fd == postmethod->target_fd)
  {
    if (postmethod->EventWrite() <= 0) {
      postmethod->kq.DisableEvent(postmethod->target_fd, EVFILT_WRITE, postmethod);
      //postmethod->kq.DeleteEvent(postmethod->target_fd, EVFILT_WRITE);

      if (postmethod->request->GetItem("Connection").value == "close")
        //delete postmethod->client;
        postmethod->kq.delete_list.insert(postmethod->client);
      //delete postmethod;
      postmethod->kq.delete_list.insert(postmethod);
    }
  }
  else
  {
    if (postmethod->FileWrite() <= 0) {
      postmethod->SetResponseMessage();
      postmethod->kq.DisableEvent(postmethod->interface_fd, EVFILT_WRITE, postmethod);
      //postmethod->kq.DeleteEvent(postmethod->interface_fd, EVFILT_WRITE);

      postmethod->kq.AddEvent(postmethod->target_fd, EVFILT_WRITE, postmethod);
    }
  }
}

void DeleteMethod_Event_Read(DeleteMethod *deletemethod)
{
  if (deletemethod->EventRead() <= 0)
  {
    deletemethod->SetResponseMessage();
    deletemethod->kq.DisableEvent(deletemethod->interface_fd, EVFILT_READ, deletemethod);
    //deletemethod->kq.DeleteEvent(deletemethod->interface_fd, EVFILT_READ);

    deletemethod->kq.AddEvent(deletemethod->target_fd, EVFILT_WRITE, deletemethod);
  }
}

void DeleteMethod_Event_Write(DeleteMethod *deletemethod)
{
  if (deletemethod->EventWrite() <= 0) {
    deletemethod->kq.DisableEvent(deletemethod->target_fd, EVFILT_WRITE, deletemethod);
    //deletemethod->kq.DeleteEvent(deletemethod->target_fd, EVFILT_WRITE);

    if (deletemethod->request->GetItem("Connection").value == "close")
      //delete deletemethod->client;
      deletemethod->kq.delete_list.insert(deletemethod->client);
    //delete deletemethod;
    deletemethod->kq.delete_list.insert(deletemethod);
  }
}

void Cgi_Event_Read(Cgi *cgi, int ident)
{
  if (ident == cgi->interface_fd) {
    if (cgi->EventRead() <= 0)
    {
      cgi->SetResponseMessage();
      cgi->kq.DisableEvent(cgi->interface_fd, EVFILT_READ, cgi);
      //cgi->kq.DeleteEvent(cgi->interface_fd, EVFILT_READ);

      cgi->kq.AddEvent(cgi->target_fd, EVFILT_WRITE, cgi);
    }
  } else {
    if (cgi->EventReadToCgi() <= 0) { // 2
      cgi->SetResponseMessageCgi();
      //cgi->kq.DisableEvent(cgi->fromCgi[FD_READ], EVFILT_READ, cgi);
      cgi->kq.DeleteEvent(cgi->fromCgi[FD_READ], EVFILT_READ);

      close(cgi->fromCgi[FD_READ]);
      // target_fd로 쓰는 이벤트 등록하기
      cgi->kq.AddEvent(cgi->target_fd, EVFILT_WRITE, cgi);
    }
  }
}

void Cgi_Event_Write(Cgi *cgi, int ident)
{
  if (ident == cgi->target_fd) {
    if (cgi->EventWrite() <= 0) {
      cgi->kq.DisableEvent(cgi->target_fd, EVFILT_WRITE, cgi);
      //cgi->kq.DeleteEvent(cgi->target_fd, EVFILT_WRITE);

      if (cgi->request->GetItem("Connection").value == "close")
        //delete cgi->client;
        cgi->kq.delete_list.insert(cgi->client);
      //delete cgi;
      cgi->kq.delete_list.insert(cgi);
    }
  } else {
    if (cgi->EventWriteToCgi() <= 0) { // request to cgi process 1
      // cgi에 다 썼으면 쓰는 이벤트 지우기
      //cgi->kq.DisableEvent(cgi->toCgi[FD_WRITE], EVFILT_WRITE, cgi);
      cgi->kq.DeleteEvent(cgi->toCgi[FD_WRITE], EVFILT_WRITE);

      close(cgi->toCgi[FD_WRITE]);
    }
  }
}

void Process(FdInterface *target, struct kevent event)
{
  if (event.flags & EV_EOF && target->interface_type == kFdClient) {
    break_point();
    target->kq.delete_list.insert(target);
    return ;
  }
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
}
