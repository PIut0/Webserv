
#include "ServerManager.hpp"
#include "KQueue.hpp"
#include "Server.hpp"
#include "process.hpp"
#include "utils.hpp"

void test(int a) {
  std::cout << "test()" << a << std::endl;
}

int main(int argc, char** argv) {
  signal(SIGPIPE, &test);
  ServerManager serverManager(CheckArg(argc, argv));
  std::vector<Server *> servers;
  KQueue kq;
  for(size_t i = 0 ; i < serverManager.serverBlock.size() ; i++)
    servers.push_back(new Server(&kq, serverManager.serverBlock[i]));
  SetMime();

  while (1) {
    kq.loof();
  }
  return (0);
}
