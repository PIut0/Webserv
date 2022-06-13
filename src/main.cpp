
#include "ServerManager.hpp"
#include "KQueue.hpp"
#include "Server.hpp"
#include "process.hpp"
#include "utils.hpp"

int main(int argc, char** argv) {
  ServerManager serverManager(CheckArg(argc, argv));
  SetMime();

  while (1) {
    try {
      KQueue kq;
      for(size_t i = 0 ; i < serverManager.serverBlock.size() ; i++)
        kq.servers.insert(new Server(&kq, serverManager.serverBlock[i]));
      kq.loof();
    } catch (const std::exception &e) {
      std::cerr << "Webserv: Restarting...: " << e.what() << std::endl;
    } catch (...) {
      std::cerr << "Webserv: Restarting..." << std::endl;
    }
  }
  return (0);
}
