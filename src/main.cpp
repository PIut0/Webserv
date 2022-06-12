
#include "ServerManager.hpp"
#include "KQueue.hpp"
#include "Server.hpp"
#include "process.hpp"
#include "utils.hpp"

int main(int argc, char** argv) {
  ServerManager serverManager(CheckArg(argc, argv));
  std::vector<Server *> servers;
  KQueue kq;
  for(size_t i = 0 ; i < serverManager.serverBlock.size() ; i++)
    servers.push_back(new Server(kq, serverManager.serverBlock[i]));
  SetMime();

  while (1) {
    kq.Refresh();
    for (int i = 0; i < kq.event_count; i++) {
      FdInterface *target = static_cast<FdInterface *>(kq.events[i].udata);
      Process(target, kq.events[i]);
    }
    kq.DeleteList();
  }
  return (0);
}
