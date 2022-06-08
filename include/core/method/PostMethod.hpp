#ifndef POSTMETHOD_HPP
#define POSTMETHOD_HPP

#include "Method.hpp"

#define PORT 8080

class Client;
class KQueue;
class ServerBlock;

class PostMethod : public Method
{
 private:
  PostMethod();
  PostMethod(const PostMethod &);
  PostMethod &operator=(const PostMethod &);
 public:
  PostMethod(KQueue &kq, const std::string &path, Client *client);
  ~PostMethod();

  int FileWrite();

  std::string target_path;
};

#endif
