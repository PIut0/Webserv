#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "ServerBlock.hpp"
#include "Defines.hpp"
#include "System.hpp"

class ServerBlock;

class ServerManager : public System
{
 public:
  ServerManager(std::string);
  ServerManager(const ServerManager &origin);
  ~ServerManager();

  ServerManager& operator=(const ServerManager &rv);

  std::vector<ServerBlock> serverBlock;

  void InitServer(std::string path);

 private:
  ServerManager();
};

#endif
