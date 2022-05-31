#ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

#include <vector>
#include <string>
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
  ServerBlock(const std::vector<std::string> &data);
  ServerBlock(const ServerBlock &origin);
  ~ServerBlock();

  ServerBlock& operator=(const ServerBlock& rv);

  void  PrintBlock();
  int   GetLocationBlockByPath(const std::string &path);

  std::vector<LocationBlock>  location;
  std::string                 server_name;
  std::string                 host;
  u_short                     port;

  enum ServerAttribute {
    kError = -1,
    kLocation,
    kServerName,
    kListen
  };

 private:
  ServerBlock();

  void InitServerBlock(const std::vector<std::string>&data);
  void CheckEssentailCommands();

  void ParseListen(const std::string &contents);
  void ParseServerName(const std::string &contents);
  ServerAttribute CheckValidate(const std::string &command);
};

#endif
