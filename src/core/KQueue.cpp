#include "KQueue.hpp"

KQueue::KQueue()
{
  timeout.tv_sec = 0;
  timeout.tv_nsec = 0;
  // Create a new kqueue
  if ((kq = kqueue()) == -1)
    ExitWithMsg("kqueue");
}

KQueue::~KQueue()
{
  close(kq);
}

void KQueue::ErrorIgnore(const char *err)
{
  std::cerr << err << std::endl;
  event_count = 0;
}

void KQueue::Refresh()
{
  //event_count = kevent(kq, NULL, 0, events, EVENT_SIZE, NULL);
  event_count = kevent(kq, &event_list[0], event_list.size(), events, EVENT_SIZE, NULL);
  event_list.clear();
  if (event_count == -1)
    ErrorIgnore("refresh");
}

void KQueue::DeleteList()
{
  // delete kq.delete_list
  std::vector<std::set<FdInterface *>::iterator> delete_list_it;
  for (std::set<FdInterface *>::iterator it = delete_list.begin(); it != delete_list.end(); it++) {
    if ((*it)->interface_type == kFdClient && static_cast<Client *>(*it)->method_list.size() > 0)
      continue;
    if (fd_map[(*it)->interface_fd] == *it)
      fd_map.erase((*it)->interface_fd);
    delete *it;
    delete_list_it.push_back(it);
  }
  for (size_t i = 0; i < delete_list_it.size(); i++) {
    delete_list.erase(delete_list_it[i]);
  }
}

void KQueue::AddEvent(int ident, int16_t filter, void *udata)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_ADD | EV_ENABLE, 0, 0, udata);

  event_list.push_back(ev);
  //event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, &timeout);
  //if (event_count == -1)
  //  ErrorIgnore("add_event");
}

void KQueue::EnableEvent(int ident, int16_t filter, void *udata)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_ENABLE, 0, 0, udata);

  event_list.push_back(ev);
  //event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, &timeout);
  //if (event_count == -1)
  //  ErrorIgnore("enable_event");
}

void KQueue::DisableEvent(int ident, int16_t filter, void *udata)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_DISABLE, 0, 0, udata);

  event_list.push_back(ev);
  //event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, &timeout);
  //if (event_count == -1)
  //  ErrorIgnore("disable_event");
}

void KQueue::DeleteEvent(int ident, int16_t filter)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_DELETE, 0, 0, NULL);
  //TODO : udata 추가

  //event_list.push_back(ev);
  event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, &timeout);
  if (event_count == -1)
    ErrorIgnore("delete_event");
}

void KQueue::AddServer(Server &server)
{
  AddEvent(server.interface_fd, EVFILT_READ, &server);
}

void KQueue::AddClient(Client *client)
{
  AddEvent(client->interface_fd, EVFILT_READ, client);
  if ((fd_map.find(client->interface_fd) != fd_map.end()) && (fd_map[client->interface_fd] != nullptr)) {
    delete_list.insert(fd_map[client->interface_fd]);
  }
  fd_map[client->interface_fd] = client;
}
