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

std::string ServerManager::CheckLine(const std::string &line)
{
  std::string str = line.substr(0, line.find('#', 0));
  char ch;

  str = rtrim(str);
  if (str.length() == 0)
    return str;
  ch = str[str.length() - 1];
  if (ch != ';' && ch != '}' && ch != '{' )
    ThrowException("config Syntax error");

  return str;
}

void ServerManager::InitServer(const std::string &path)
{
  std::string line;
  std::ifstream config_file;
  std::vector<std::string> serverBlock;
  u_short state = S_DEFAULT;

  config_file.open(path);
  if (!config_file.is_open())
    ThrowException("file open error");

  while (std::getline(config_file, line)) {
    line = CheckLine(line);

    switch (state)
    {
      case S_DEFAULT:
        if (line == SERVER_BLOCK_OPEN) {
          state <<= 1;
        }
        break;

      case S_SERVER:
        if (line == SERVER_BLOCK_CLOSE) {
          this->serverBlock.push_back(ServerBlock(serverBlock));
          CLEAR_VECTOR_COMPLETLY(serverBlock)
          state >>= 1;
          break;
        }

        if (line.length() > 0) {
          serverBlock.push_back(line);
        }
        break;

      default:
        break;
    }
  }
}
