#include "ServerManager.hpp"

ServerManager::ServerManager(std::string path)
{
  InitServer(path);
}

ServerManager::ServerManager(const ServerManager &origin)
{
  *this = origin;
}



ServerManager::~ServerManager() {}

ServerManager& ServerManager::operator=(const ServerManager &rv)
{
  this->serverBlock = rv.serverBlock;
  return *this;
}

void ServerManager::InitServer(std::string path)
{
  std::string line;
  std::ifstream config_file;
  std::vector<std::string> temp;
  u_short state = S_DEFAULT;

  config_file.open(path);
  if (!config_file.is_open())
    exit(1);
  while (std::getline(config_file, line))
  {
    line = line.substr(0, line.find('#', 0));
    if (state & S_DEFAULT && line == SERVER_BLOCK_OPEN)
    {
      // this->getMonitor().print(COLOR_GREEN, "new server block open");
      state <<= 1;
    }
    else if (state & S_SERVER && line == SERVER_BLOCK_CLOSE)
    {
      this->serverBlock.push_back(ServerBlock(temp));
      CLEAR_VECTOR_COMPLETLY(temp)
      state >>= 1;
    }
    else if (state & S_SERVER && line.length() > 0)
    {
      temp.push_back(line);
    }
  }
  // this->getMonitor().print(COLOR_RED, "server block close");
}
