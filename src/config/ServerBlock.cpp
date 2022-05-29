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

void ServerBlock::ParseListen(const std::string &data)
{
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  this->port = atoi(split_data[0].c_str());
  this->host = split_data[1];
}
void ServerBlock::ParseServerName(const std::string &data)
{
  this->server_name = data;
}

ServerBlock::ServerAttribute ServerBlock::CheckValidate(const std::string &command, const std::string &contents)
{
  (void)contents;
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
        index = data[i].find(' ');

        command = data[i].substr(1, index - 1);
        contents = data[i].substr(index + 1, data[i].find(';') - index - 1);

        type = CheckValidate(command, contents);

        switch (type)
        {
          case kLocation:
            location_path = data[i].substr(10, data[i].find('{', 0) - 10);
            state <<= 1;
            break;

          case kServerName:
            ParseServerName(contents);
            break;

          case kListen:
            ParseListen(contents);
            break;

          case kError:
            ExitWithPerror("error in serverblock");

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
