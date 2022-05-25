#include "RequestHeader.hpp"
#include <iostream>
RequestHeader::RequestHeader() {}

RequestHeader::~RequestHeader() {}

void RequestHeader::SetItem(std::string &key, std::string &value)
{
  if (FindItem(key) != this->conf.end())
    throw AlreadyExistKey();

  wsv_header_t *el = new wsv_header_t();
  el->key = key;
  el->value = value;
  this->conf[key] = el;
}

req_header_it_t RequestHeader::FindItem(std::string &key)
{
  return this->conf.find(key);
}

wsv_header_t& RequestHeader::GetItem(std::string &key)
{
  return *(FindItem(key)->second);
}
