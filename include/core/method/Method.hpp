#ifndef METHOD_HPP
#define METHOD_HPP

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <vector>

#include "KQueue.hpp"
#include "FdInterface.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "ServerBlock.hpp"

#define PORT 8080

class Client;
class KQueue;
class ServerBlock;

class Method : public FdInterface
{
 private:
  Method();
  Method(const Method &);
  Method &operator=(const Method &);
 public:
  Method(KQueue &kq, Client *client, FdInterfaceType type);
  virtual ~Method();

  int EventRead();
  int EventWrite();
  int IsDir(const std::string &path);
  void SetResponseStatus(ResponseHeader *response, const int code);
  void ResponseErrorPage();
  void SetResponseMessage();

  int target_fd;
  int response_idx;
  int response_data_size;
  std::string target_path;
  std::string data;
  std::string response_data;
  Client *client;
  RequestHeader *request;
  ResponseHeader *response;
  LocationBlock *location;
};

#endif
