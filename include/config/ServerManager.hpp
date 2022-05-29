#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "ServerBlock.hpp"
#include "Defines.hpp"
#include "System.hpp"
#include "utils.hpp"

class ServerBlock;

class ServerManager : public System
{
 public:
  ServerManager(std::string);
  ServerManager(const ServerManager &origin);
  ~ServerManager();

  ServerManager& operator=(const ServerManager &rv);

  void        InitServer(const std::string &path);
  std::string CheckLine(const std::string &line);

  std::vector<ServerBlock> serverBlock;

 private:
  ServerManager();
};

#endif
