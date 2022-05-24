#include <vector>
#include <string>

#include "ServerManager.hpp"
#include "utils.hpp"
#include "KQueue.hpp"
#include "Server.hpp"
#include "process.hpp"

int main(int argc, char** argv) {
  ServerManager serverManager(CheckArg(argc, argv));
  std::vector<Server *> servers;
  KQueue kq;
  for(size_t i = 0 ; i < serverManager.serverBlock.size() ; i++) {
    servers.push_back(new Server(kq, serverManager.serverBlock[i]));
    kq.AddServer(*(servers[i]));
  }

  while (1) {
    kq.Refresh();
    for (int i = 0; i < kq.event_count; i++) {
      Process(static_cast<FdInterface *>(kq.events[i].udata), kq.events[i]);
    }
  }
  return (0);
}
