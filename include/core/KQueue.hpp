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

#include "FdInterface.hpp"
#include "HttpStatusCode.hpp"
#include "utils.hpp"
#include "Server.hpp"

#define EVENT_SIZE 64

class Server;
class Client;
class FdInterface;

typedef std::map<int, Client> client_map_t;
typedef client_map_t::iterator client_map_it_t;

typedef std::set<int> delete_list_t;
typedef delete_list_t::iterator delete_list_it_t;

class KQueue
{
 private:
 public:
  KQueue();
  KQueue &operator=(const KQueue &other);
  KQueue(const KQueue &other);
  ~KQueue();

  void loof();

  void ErrorIgnore(const char *err);
  void Refresh();
  void DeleteList();
  void AddEvent(int ident, int16_t filter, void *udata);
  void EnableEvent(int ident, int16_t filter, void *udata);
  void DisableEvent(int ident, int16_t filter, void *udata);
  void DeleteEvent(int ident, int16_t filter, void *udata);
  void AddServer(Server &serv);
  void AddClient(int fd, Server *server);

  struct timespec timeout;
  int           kq;
  int           event_count;
  std::vector<struct kevent> event_list;
  std::set<Server *> servers;
  client_map_t client_map;
  delete_list_t delete_list;

  struct kevent events[EVENT_SIZE];
};

#endif
