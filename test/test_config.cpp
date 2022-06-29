#include "ServerManager.hpp"

int main(int argc, char **argv)
{
  ServerManager serverManager(CheckArg(argc, argv));

  for (size_t i = 0 ; i < serverManager.serverBlock.size() ; ++i) {
    serverManager.serverBlock[i].PrintBlock();
  }
  return 0;
}
