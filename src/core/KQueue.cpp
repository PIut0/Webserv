#include "KQueue.hpp"

KQueue::KQueue()
{
  timeout.tv_sec = 0;
  timeout.tv_nsec = 0;
  // Create a new kqueue
  if ((kq = kqueue()) == -1)
    ExitWithPerror("kqueue");
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
  event_count = kevent(kq, NULL, 0, events, EVENT_SIZE, NULL);
  if (event_count == -1)
    ErrorIgnore("refresh");
}

void KQueue::AddEvent(int ident, int16_t filter, void *udata)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_ADD | EV_ENABLE, 0, 0, udata);

  event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, &timeout);
  if (event_count == -1)
    ErrorIgnore("add_event");
}

void KQueue::EnableEvent(int ident, int16_t filter, void *udata)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_ENABLE, 0, 0, udata);

  event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, &timeout);
  if (event_count == -1)
    ErrorIgnore("enable_event");
}

void KQueue::DisableEvent(int ident, int16_t filter, void *udata)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_DISABLE, 0, 0, udata);

  event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, &timeout);
  if (event_count == -1)
    ErrorIgnore("disable_event");
}

void KQueue::DeleteEvent(int ident, int16_t filter)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_DELETE, 0, 0, &timeout);

  event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, &timeout);
  if (event_count == -1)
    ErrorIgnore("delete_event");
}

void KQueue::AddServer(Server &server)
{
  AddEvent(server.interface_fd, EVFILT_READ, &server);
}
