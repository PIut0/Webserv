#include "KQueue.hpp"

KQueue::KQueue()
{
  // Create a new kqueue
  if ((kq = kqueue()) == -1)
    ExitWithPerror("kqueue");
}

KQueue::~KQueue()
{
  close(kq);
}

void KQueue::Refresh()
{
  event_count = kevent(kq, NULL, 0, events, EVENT_SIZE, NULL);
  if (event_count == -1)
    ExitWithPerror("refresh");
}

void KQueue::AddEvent(int ident, int16_t filter, void *udata)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_ADD | EV_ENABLE, 0, 0, udata);

  event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, NULL);
  if (event_count == -1)
    ExitWithPerror("add_event");
}

void KQueue::DeleteEvent(int ident, int16_t filter)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_DELETE, 0, 0, NULL);

  event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, NULL);
  if (event_count == -1)
    ExitWithPerror("delete_event");
}

void KQueue::AddServer(Server &server)
{
  AddEvent(server.socket_fd, EVFILT_READ, &server);
}
