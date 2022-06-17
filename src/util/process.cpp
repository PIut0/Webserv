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
    client->kq->delete_list.insert(client->interface_fd);
    return;
  }

  FdInterfaceType type = client->ParseReq();
  if (type == kFdNone)
    return;
  switch(type) {
    case kFdGetMethod:
      client->cur_method.SetMethod(kFdGetMethod);
      break;

    case kFdPutMethod:
      client->cur_method.SetMethod(kFdPutMethod);
      break;

    case kFdPostMethod:
      client->cur_method.SetMethod(kFdPostMethod);
      break;

    case kFdDeleteMethod:
      client->cur_method.SetMethod(kFdDeleteMethod);
      break;

    case kFdCgi:
      client->cur_method.SetMethod(kFdCgi);
      break;

    default:
      break;
  }
  client->kq->DisableEvent(client->interface_fd, EVFILT_READ, client);
}

void Client_Event_Write(Client *client)
{
  client->EventWrite();
}

void GetMethod_Event_Read(Method *getmethod)
{
  Client &client = getmethod->client;

  if (getmethod->EventRead() <= 0) {
    getmethod->SetResponseMessage();
    CloseFd(getmethod->interface_fd);
    //client.kq->DeleteEvent(getmethod->interface_fd, EVFILT_READ, getmethod);
    client.kq->AddEvent(client.interface_fd, EVFILT_WRITE, getmethod);
  }
}

void GetMethod_Event_Write(Method *getmethod)
{
  Client &client = getmethod->client;

  if (getmethod->EventWrite() <= 0) {
    client.kq->DisableEvent(client.interface_fd, EVFILT_WRITE, getmethod);

    if (client.request.GetItem("Connection").value == "close") {
      client.kq->delete_list.insert(client.interface_fd);
    } else {
      client.kq->EnableEvent(client.interface_fd, EVFILT_READ, &(client));
    }
    client.cur_method.Clear();
  }
}

void PutMethod_Event_Read(Method *putmethod)
{
  Client &client = putmethod->client;

  if (putmethod->EventRead() <= 0) {
    putmethod->SetResponseMessage();
    CloseFd(putmethod->interface_fd);
    //client.kq->DeleteEvent(putmethod->interface_fd, EVFILT_READ, putmethod);
    client.kq->AddEvent(client.interface_fd, EVFILT_WRITE, putmethod);
  }
}

void PutMethod_Event_Write(Method *putmethod, int fd)
{
  Client &client = putmethod->client;

  if (fd == client.interface_fd) {
    if (putmethod->EventWrite() <= 0) {
      client.kq->DisableEvent(client.interface_fd, EVFILT_WRITE, putmethod);

      if (client.request.GetItem("Connection").value == "close") {
        client.kq->delete_list.insert(client.interface_fd);
      } else {
        client.kq->EnableEvent(client.interface_fd, EVFILT_READ, &(client));
      }
      client.cur_method.Clear();
    }
  }
  else {
    if (putmethod->FileWrite() <= 0) {
      putmethod->SetResponseMessage();
      CloseFd(putmethod->interface_fd);
      //client.kq->DeleteEvent(putmethod->interface_fd, EVFILT_WRITE, putmethod);
      client.kq->AddEvent(client.interface_fd, EVFILT_WRITE, putmethod);
    }
  }
}

void PostMethod_Event_Read(Method *postmethod)
{
  Client &client = postmethod->client;

  if (postmethod->EventRead() <= 0) {
    postmethod->SetResponseMessage();
    CloseFd(postmethod->interface_fd);
    //client.kq->DeleteEvent(postmethod->interface_fd, EVFILT_READ, postmethod);
    client.kq->AddEvent(client.interface_fd, EVFILT_WRITE, postmethod);
  }
}

void PostMethod_Event_Write(Method *postmethod, int fd)
{
  Client &client = postmethod->client;

  if (fd == client.interface_fd) {
    if (postmethod->EventWrite() <= 0) {
      client.kq->DisableEvent(client.interface_fd, EVFILT_WRITE, postmethod);

      if (client.request.GetItem("Connection").value == "close") {
        client.kq->delete_list.insert(client.interface_fd);
      } else {
        client.kq->EnableEvent(client.interface_fd, EVFILT_READ, &(client));
      }
      client.cur_method.Clear();
    }
  }
  else {
    if (postmethod->FileWrite() <= 0) {
      postmethod->SetResponseMessage();
      CloseFd(postmethod->interface_fd);
      //client.kq->DeleteEvent(postmethod->interface_fd, EVFILT_WRITE, postmethod);
      client.kq->AddEvent(client.interface_fd, EVFILT_WRITE, postmethod);
    }
  }
}

void DeleteMethod_Event_Read(Method *deletemethod)
{
  Client &client = deletemethod->client;

  if (deletemethod->EventRead() <= 0) {
    deletemethod->SetResponseMessage();
    CloseFd(deletemethod->interface_fd);
    //client.kq->DeleteEvent(deletemethod->interface_fd, EVFILT_READ, deletemethod);
    client.kq->AddEvent(client.interface_fd, EVFILT_WRITE, deletemethod);
  }
}

void DeleteMethod_Event_Write(Method *deletemethod)
{
  Client &client = deletemethod->client;

  if (deletemethod->EventWrite() <= 0) {
    client.kq->DisableEvent(client.interface_fd, EVFILT_WRITE, deletemethod);

    if (client.request.GetItem("Connection").value == "close") {
      client.kq->delete_list.insert(client.interface_fd);
    } else {
      client.kq->EnableEvent(client.interface_fd, EVFILT_READ, &(client));
    }
    client.cur_method.Clear();
  }
}

void Cgi_Event_Read(Method *cgi, int ident)
{
  Client &client = cgi->client;

  if (ident == cgi->interface_fd) {
    if (cgi->EventRead() <= 0)
    {
      cgi->SetResponseMessage();
      CloseFd(cgi->interface_fd);
      //client.kq->DeleteEvent(cgi->interface_fd, EVFILT_READ, cgi);
      client.kq->AddEvent(client.interface_fd, EVFILT_WRITE, cgi);
    }
  }
  else {
    if (cgi->EventReadToCgi() <= 0) { // 2
      cgi->SetResponseMessageCgi();
      CloseFd(cgi->from_cgi[FD_READ]);
      //client.kq->DisableEvent(cgi->from_cgi[FD_READ], EVFILT_READ, cgi);
      client.kq->AddEvent(client.interface_fd, EVFILT_WRITE, cgi);
    }
  }
}

void Cgi_Event_Write(Method *cgi, int ident)
{
  Client &client = cgi->client;

  if (ident == client.interface_fd) {
    if (cgi->EventWrite() <= 0) {
      client.kq->DisableEvent(client.interface_fd, EVFILT_WRITE, cgi);

      if (client.request.GetItem("Connection").value == "close") {
        client.kq->delete_list.insert(client.interface_fd);
      } else {
        client.kq->EnableEvent(client.interface_fd, EVFILT_READ, &(client));
      }
      client.cur_method.Clear();
    }
  }
  else {
    if (cgi->EventWriteToCgi() <= 0) { // request to cgi process 1
      CloseFd(cgi->to_cgi[FD_WRITE]);
      //client.kq->DeleteEvent(cgi->to_cgi[FD_WRITE], EVFILT_WRITE, cgi);
    }
  }
}

void Process(FdInterface* &target, struct kevent event)
{
  if (event.flags & EV_EOF
    && event.filter == EVFILT_READ
    && target->interface_type == kFdClient
    && target->kq->client_map.find(event.ident) != target->kq->client_map.end()) {
    // CloseFd(event.ident);
    target->kq->delete_list.insert(event.ident);
    return;
  }
  //if (event.flags & EV_EOF && event.filter == EVFILT_READ) {
  //  CloseFd(event.ident);
  //  if (target->interface_type == kFdClient && target->kq->client_map.find(event.ident) != target->kq->client_map.end())
  //    target->kq->delete_list.insert(event.ident);
  //  return;
  //}
  if (event.filter == EVFILT_READ) {
    switch (target->interface_type) {
      case kFdServer:
        Server_Event_Read(static_cast<Server *>(target));
        break;

      case kFdClient:
        Client_Event_Read(static_cast<Client *>(target));
        break;

      case kFdGetMethod:
        GetMethod_Event_Read(static_cast<Method *>(target));
        break;

      case kFdPutMethod:
        PutMethod_Event_Read(static_cast<Method *>(target));
        break;

      case kFdPostMethod:
        PostMethod_Event_Read(static_cast<Method *>(target));
        break;

      case kFdDeleteMethod:
        DeleteMethod_Event_Read(static_cast<Method *>(target));
        break;

      case kFdCgi:
        Cgi_Event_Read(static_cast<Method *>(target), event.ident);
        break;

      default:
        break;
    }
  } else if (event.filter == EVFILT_WRITE) {
    switch (target->interface_type) {
      case kFdServer:
        Server_Event_Write(static_cast<Server *>(target));
        break;

      case kFdClient:
        Client_Event_Write(static_cast<Client *>(target));
        break;

      case kFdGetMethod:
        GetMethod_Event_Write(static_cast<Method *>(target));
        break;

      case kFdPutMethod:
        PutMethod_Event_Write(static_cast<Method *>(target), event.ident);
        break;

      case kFdPostMethod:
        PostMethod_Event_Write(static_cast<Method *>(target), event.ident);
        break;

      case kFdDeleteMethod:
        DeleteMethod_Event_Write(static_cast<Method *>(target));
        break;

      case kFdCgi:
        Cgi_Event_Write(static_cast<Method *>(target), event.ident);
        break;

      default:
        break;
    }
  }
}
