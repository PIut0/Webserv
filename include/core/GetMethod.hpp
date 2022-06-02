#ifndef GETMETHOD_HPP
#define GETMETHOD_HPP

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>

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
};

#endif
