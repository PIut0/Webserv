#include <vector>
#include <string>

#include "ServerManager.hpp"
#include "utils.hpp"
#include "KQueue.hpp"
#include "Server.hpp"

int main(int argc, char** argv) {
  ServerManager serverManager(CheckArg(argc, argv));
  std::vector<Server *> servers;
  KQueue kq;
  for(size_t i = 0 ; i < serverManager.serverBlock.size() ; i++) {
    servers.push_back(new Server(kq, serverManager.serverBlock[i]));
    kq.AddServer(*(servers[i]));
  }

  int status;

  while (1) {
    kq.Refresh();
    for (int i = 0; i < kq.event_count; i++) {
      FdInterface *target = static_cast<FdInterface *>(kq.events[i].udata);
      if (kq.events[i].filter == EVFILT_READ){
        if ((status = target->EventRead()) <= 0
          && !static_cast<Client *>(target)->has_body) {
          delete target;
          std::cout << "fd: " << kq.events[i].ident << ": delete client" << std::endl;
          close(target->interface_fd);
        }
      }
      else if (kq.events[i].filter == EVFILT_WRITE) {
        if ((target->EventWrite()) <= 0) {
          kq.DeleteEvent(kq.events[i].ident, kq.events[i].filter);
        }
      }
    }
  }
  return (0);
}
