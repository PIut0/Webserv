#ifndef KQUEUE_HPP
#define KQUEUE_HPP

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <set>
#include <unistd.h>
#include <fcntl.h>

#include "utils.hpp"
#include "Server.hpp"

#define EVENT_SIZE 64

class Server;
class FdInterface;

class KQueue
{
 private:
  KQueue(const KQueue &);
  KQueue &operator=(const KQueue &);
 public:
  KQueue();
  ~KQueue();

  void ErrorIgnore(const char *err);
  void Refresh();
  void DeleteList();
  void AddEvent(int ident, int16_t filter, void *udata);
  void EnableEvent(int ident, int16_t filter, void *udata);
  void DisableEvent(int ident, int16_t filter, void *udata);
  void DeleteEvent(int ident, int16_t filter);
  void AddServer(Server &serv);


  struct timespec timeout;
  int           kq;
  int           event_count;
  std::vector<struct kevent> event_list;
  std::set<FdInterface *> delete_list;
  struct kevent events[EVENT_SIZE];
};

#endif
