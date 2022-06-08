#ifndef GETMETHOD_HPP
#define GETMETHOD_HPP

#include "Method.hpp"

#define PORT 8080

class Client;
class KQueue;
class ServerBlock;

class GetMethod : public Method
{
 private:
  GetMethod();
  GetMethod(const GetMethod &);
  GetMethod &operator=(const GetMethod &);
 public:
  GetMethod(KQueue &kq, const std::string &path, Client *client);
  ~GetMethod();

  std::vector<std::string> file_list;
};

#endif
