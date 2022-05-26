#include "ResponseHeader.hpp"


ResponseHeader::ResponseHeader() {}

ResponseHeader::~ResponseHeader() {}

void ResponseHeader::SetItem(std::string &key, std::string &value)
{
  (void)key;
  (void)value;
}

res_header_it_t ResponseHeader::FindItem(std::string &key)
{
  res_header_it_t it;

  for (it = this->conf.begin() ; it != this->conf.end() ; ++it) {
    if ((*it)->key == key) break;
  }
  return it;
}

wsv_header_t& ResponseHeader::GetItem(std::string &key)
{
  return *(*FindItem(key));
}
