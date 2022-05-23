#include "KQueue.hpp"

KQueue::KQueue()
{
  // Create a new kqueue
  if ((kq = kqueue()) == -1)
    exit_with_perror("kqueue");
}

KQueue::~KQueue()
{
  close(kq);
}

void KQueue::refresh()
{
  event_count = kevent(kq, NULL, 0, events, EVENT_SIZE, NULL);
  if (event_count == -1)
    exit_with_perror("refresh");
}

void KQueue::add_event(int ident, int16_t filter, void *udata)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_ADD | EV_ENABLE, 0, 0, udata);

  event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, NULL);
  if (event_count == -1)
    exit_with_perror("add_event");
}

void KQueue::delete_event(int ident, int16_t filter)
{
  struct kevent ev;
  EV_SET(&ev, ident, filter, EV_DELETE, 0, 0, NULL);

  event_count = kevent(kq, &ev, 1, events, EVENT_SIZE, NULL);
  if (event_count == -1)
    exit_with_perror("delete_event");
}

void KQueue::add_server(Server &server)
{
  add_event(server.socket_fd, EVFILT_READ, &server);
}
