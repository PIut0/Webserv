#ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

#include "LocationBlock.hpp"
#include "Defines.hpp"
#include "System.hpp"
#include "Color.hpp"

class LocationBlock;

class ServerBlock : public System
{
public:
  ServerBlock(const std::vector<std::string>&);
  ~ServerBlock();

  std::vector<LocationBlock>	location;
  std::string					server_name;
  std::string					host;
  u_short						port;

  enum ServerAttribute {
    ERROR_ = -1,
    LOCATION_,
    SERVER_NAME_,
    LISTEN_
  };

private:
  ServerBlock();

  void InitServerBlock(const std::vector<std::string>&);
  void CheckEssentailCommands();
  void PrintBlock();

  ServerAttribute ParseListen(const std::string&);
  ServerAttribute ParseServerName(const std::string&);
  ServerAttribute CheckValidate(const std::string&);
};

#endif
