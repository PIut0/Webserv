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

  void init_server_block(const std::vector<std::string>&);
  void check_essentail_commands();
  void printBlock();

  ServerAttribute parseListen(const std::string&);
  ServerAttribute parseServerName(const std::string&);
  ServerAttribute check_validate(const std::string&);
};

#endif
