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

void RequestHeader::Print()
{
  std::cout << "[method] : ";
  switch (this->method)
  {
    case HTTP_UNKNOWN:
      std::cout << "UNKNOWN";
      break;

    case HTTP_GET:
      std::cout << "GET";
      break;

    case HTTP_POST:
      std::cout << "POST";
      break;

    case HTTP_PUT:
      std::cout << "PUT";
      break;

    case HTTP_DELETE:
      std::cout << "DELETE";
      break;

    default:
      break;
  }
  std::cout << std::endl;
  std::cout << "[host] : " << strndup(this->host_start, this->host_end - this->host_start) << std::endl;
  std::cout << "[http version] : " << this->http_major << "." << this->http_minor << std::endl << std::endl;
  PrintItem();
}

void RequestHeader::PrintItem()
{
  req_header_it_t it;

  for (it = this->conf.begin() ; it != this->conf.end() ; ++it) {
    std::cout << "[" << it->second->key << "] : " << it->second->value << std::endl;
  }
}
