#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Method.hpp"
#include "Server.hpp"
#include "FdInterface.hpp"
#include "RequestHeader.hpp"
#include "ResponseHeader.hpp"

#define FD_READ  0
#define FD_WRITE 1

#define PS_CHILD 0

class Client;
class Server;
class KQueue;
class LocationBlock;

class KQueue;

class Method : public FdInterface
{
 private:
  Method();
 public:
  Method &operator=(const Method &);
  Method(const Method &);
  Method(Client &client);
  virtual ~Method();

  void SetMethod(FdInterfaceType type);
  void Clear();

  int EventRead();
  int EventWrite();
  int FileWrite();
  int IsDir(const std::string &path);
  void SetResponseStatus(const int code);
  void ResponseErrorPage();
  void SetResponseMessage();

  int EventReadToCgi();
  int EventWriteToCgi();
  void SetResponseMessageCgi();

  int from_cgi[2];
  int to_cgi[2];
  int target_fd;

  std::string read_data;

  int cgi_write_data_idx;
  int cgi_write_data_size;
  std::string cgi_write_data;

  int write_data_idx;
  int write_data_size;
  std::string write_data;

  std::string target_path;

  Client &client;
  LocationBlock *location;
};

int IsDir(const std::string &path);

void GetMethod(Method *method);
void PutMethod(Method *method);
void PostMethod(Method *method);
void DeleteMethod(Method *method);
void CgiMethod(Method *method);

class Client : public FdInterface
{
 public:
  Client();
  Client(KQueue *kq, int fd, Server *server);
  Client(const Client &origin);
  ~Client();

  Client &operator=(const Client &rv);

  int EventRead();
  int EventWrite();

  LocationBlock     *GetLocationBlock();
  const std::string GetFilePath();

  int CheckCgi();
  int CheckRequest();

  FdInterfaceType ParseHeader();
  FdInterfaceType ParseBody();
  FdInterfaceType ParseReq();

  void SetResponseMessage();
  void SetSocketHitTime();

  void Clear();

  Method cur_method;
  Server *server;
  RequestHeader request;
  ResponseHeader response;
  std::string request_message;
};

#endif
