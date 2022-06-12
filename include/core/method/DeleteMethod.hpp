#ifndef DELETEMETHOD_HPP
#define DELETEMETHOD_HPP

#include "Method.hpp"

#define PORT 8080

class Client;
class KQueue;
class ServerBlock;

class DeleteMethod : public Method
{
 private:
  DeleteMethod();
  DeleteMethod(const DeleteMethod &);
  DeleteMethod &operator=(const DeleteMethod &);
 public:
  DeleteMethod(KQueue &kq, const std::string &path, Client* &client);
  ~DeleteMethod();
};

#endif
