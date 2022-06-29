#include "HTTPConfig.hpp"

HTTPConfig::HTTPConfig()
{

}

HttpContentsType HTTPConfig::FindType(std::string &command)
{
  return this->data.find(command)->second;
}
