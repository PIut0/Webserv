#include "KQueue.hpp"
#include "process.hpp"
#include <errno.h>

KQueue::KQueue()
{
  timeout.tv_sec = 0;
  timeout.tv_nsec = 0;
  // Create a new kqueue
  if ((kq = kqueue()) == -1)
    ExitWithMsg("kqueue");
}

KQueue &KQueue::operator=(const KQueue &)
{
  //kq = other.kq;
  return (*this);
}

KQueue::KQueue(const KQueue &other)
{
  *this = other;
}

KQueue::~KQueue()
{
  //CloseFd(kq);
}

void KQueue::loof()
{
  Refresh();
  for (int i = 0; i < event_count; i++) {
    FdInterface *target = static_cast<FdInterface *>(events[i].udata);
    Process(target, events[i]);
  }
  DeleteList();
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
  if (event_count == -1) {
    ErrorIgnore("refresh");
    std::cout << errno << std::endl;
  }
}

void KQueue::DeleteList()
{
  for (delete_list_it_t it = delete_list.begin(); it != delete_list.end(); it++) {
    client_map.erase(*it);
  }
  delete_list.clear();
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

void KQueue::DeleteEvent(int ident, int16_t filter, void *udata)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_DELETE, 0, 0, udata);
  //TODO : udata 추가

  event_list.push_back(ev);
  //event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, &timeout);
  //if (event_count == -1)
  //  ErrorIgnore("delete_event");
}

void KQueue::AddServer(Server &server)
{
  AddEvent(server.interface_fd, EVFILT_READ, &server);
}

void KQueue::AddClient(int fd, Server *server)
{
  Client client(this, fd ,server);
  //client_map.insert(std::make_pair(fd, client));
  client_map[fd] = client;
  AddEvent(fd, EVFILT_READ, &client_map[fd]);
  client.Clear();
}
