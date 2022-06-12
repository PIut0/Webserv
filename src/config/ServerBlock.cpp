#include "ServerBlock.hpp"

ServerBlock::ServerBlock(const std::vector<std::string> &data)
{
  InitServerBlock(data);
}

ServerBlock::ServerBlock(const ServerBlock &origin)
{
  *this = origin;
}

ServerBlock::~ServerBlock() {}

ServerBlock& ServerBlock::operator=(const ServerBlock& rv)
{
  this->location = rv.location;
  this->server_name = rv.server_name;
  this->host = rv.host;
  this->port = rv.port;
  return *this;
}

int ServerBlock::GetLocationBlockByPath(const std::string &path)
{
  std::string part;
  std::string line;
  int idx;
  size_t max_length;

  max_length = 0;
  idx = -1;
  line = path[path.length() -1] == '/' ? path.substr(0, path.length() - 1) : path;
  for (size_t i = 0 ; i < this->location.size() ; ++i) {
    if (this->location[i].location_path == path) {
      return i;
    }
    part = path.substr(0, this->location[i].location_path.length());
    if (part == this->location[i].location_path && max_length < part.length()) {
      max_length = part.length();
      idx = i;
    }
  }
  return idx;
}

void ServerBlock::ParseListen(const std::string &contents)
{
  std::vector<std::string> split_data = StringSplit(contents, " ", 0);
  char *ptr;
  this->port = strtod(split_data[0].c_str(), &ptr);
  if (this->port <= 0 || ptr[0])
    ExitWithMsg("listen Error");
  this->host = split_data[1];
}
void ServerBlock::ParseServerName(const std::string &contents)
{
  this->server_name = contents;
}

ServerBlock::ServerAttribute ServerBlock::CheckValidate(const std::string &command)
{
  if (command == "server_name")
    return kServerName;
  else if (command == "listen")
    return kListen;
  else if (command == "location")
    return kLocation;
  return kError;
}

void ServerBlock::InitServerBlock(const std::vector<std::string> &data)
{
  std::string command, contents;
  std::string location_path = "";
  std::vector<std::string> element;
  ServerAttribute type;
  size_t  index;
  u_short state = S_SERVER;


  for (size_t i = 0 ; i < data.size() ; i++) {
    switch (state)
    {
      case S_SERVER:
        if (data[i][0] != '\t')
          ExitWithMsg("Server Block Error");

        index = data[i].find(' ');

        command = data[i].substr(1, index - 1);
        contents = data[i].substr(index + 1, data[i].find(';') - index - 1);

        type = CheckValidate(command);

        switch (type)
        {
          case kLocation:
            location_path = data[i].substr(10, data[i].find('{', 0) - 11);
            state <<= 1;
            break;

          case kServerName:
            ParseServerName(contents);
            break;

          case kListen:
            ParseListen(contents);
            break;

          case kError:
            ExitWithMsg("error in serverblock");

          default:
            break;
        }

        break;

      case S_LOCATION:
        if (data[i].substr(0, 2) == LOCATION_BLOCK_CLOSE) {
          this->location.push_back(LocationBlock(location_path, element));
                  CLEAR_VECTOR_COMPLETLY(element);
          state >>= 1;
          break;
        }

        element.push_back(data[i]);

        break;

      default:
        break;
    }
  }
}

void ServerBlock::PrintBlock()
{
  std::cout << COLOR_RED << "[ SERVER BLOCK START ]" << COLOR_DEFAULT << std::endl;
  std::cout << SERVER_BLOCK_TAP << "server_name : " << this->server_name << std::endl;
  std::cout << SERVER_BLOCK_TAP << "host : " << this->host << std::endl;
  std::cout << SERVER_BLOCK_TAP << "port : " << this->port << std::endl;
  for (size_t i = 0 ; i < this->location.size() ; ++i) {
    this->location[i].PrintBlock();
  }
  std::cout << COLOR_RED << "[ SERVER BLOCK END ]" << COLOR_DEFAULT << std::endl;
}
