#ifndef PUTMETHOD_HPP
#define PUTMETHOD_HPP

#include "Method.hpp"

#define PORT 8080

class Client;
class KQueue;
class ServerBlock;

class PutMethod : public Method
{
 private:
  PutMethod();
  PutMethod(const PutMethod &);
  PutMethod &operator=(const PutMethod &);
 public:
  PutMethod(KQueue &kq, const std::string &path, Client *client);
  ~PutMethod();

  int FileWrite();

  std::string target_path;
  std::vector<std::string> file_list;
};

#endif
