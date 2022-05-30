
#include "ServerManager.hpp"
#include "KQueue.hpp"
#include "Server.hpp"
#include "Fileio.hpp"

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
      Fileio *socket_class = static_cast<Fileio *>(kq.events[i].udata);
      if (kq.events[i].filter == EVFILT_READ){
        if ((status = socket_class->EventRead()) <= 0
          && !static_cast<Fileio *>(socket_class)->client.has_body) {
          delete socket_class;
          std::cout << "fd: " << kq.events[i].ident << ": delete client" << std::endl;
          close(socket_class->interface_fd);
        }
      }
      else if (kq.events[i].filter == EVFILT_WRITE) {
        if ((socket_class->EventWrite()) <= 0) {
          kq.DeleteEvent(kq.events[i].ident, kq.events[i].filter);
        }
      }
    }
  }
  return (0);
}
