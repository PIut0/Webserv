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

ServerBlock::ServerAttribute ServerBlock::ParseListen(const std::string &data)
{
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  this->port = atoi(split_data[0].c_str());
  this->host = split_data[1];
  return kListen;
}
ServerBlock::ServerAttribute ServerBlock::ParseServerName(const std::string &data)
{
  this->server_name = data;
  return kServerName;
}

ServerBlock::ServerAttribute ServerBlock::CheckValidate(const std::string &data)
{

  int index = data.find(' ');
  std::string command = data.substr(1, index - 1);
  std::string contents = data.substr(index + 1, data.find(';') - index - 1);

  if (data[0] != '\t')
    return kError;
  if (command == "server_name")
    return ParseServerName(contents);
  else if (command == "listen")
    return ParseListen(contents);
  else if (command == "location")
    return kLocation;
  return kError;
}

void ServerBlock::InitServerBlock(const std::vector<std::string> &data)
{
  std::vector<std::string> element;
  ServerAttribute type;

  std::string location_path = "";
  u_short state = S_SERVER;

  for (size_t i = 0 ; i < data.size() ; i++) {
    switch (state)
    {
    case S_SERVER:
      if ((type = CheckValidate(data[i])) == kError)
        ExitWithPerror("error");
      if (type == kLocation) {
        location_path = data[i].substr(10, data[i].find('{', 0) - 10);
        state <<= 1;
      } else {
        if ((type = CheckValidate(data[i])) == kError)
          ExitWithPerror("error");
      }
      break;
    case S_LOCATION:
      if (data[i].substr(0, 2) == LOCATION_BLOCK_CLOSE) {
        this->location.push_back(LocationBlock(location_path, element));
                CLEAR_VECTOR_COMPLETLY(element);
        state >>= 1;
      } else {
        element.push_back(data[i]);
      }
    default:
      break;
    }
  }
}

void ServerBlock::PrintBlock()
{
  std::cout << "server_name : " << this->server_name << std::endl;
  std::cout << "host : " << this->host << std::endl;
  std::cout << "port : " << this->port << std::endl;
}
