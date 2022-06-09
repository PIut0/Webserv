#ifndef CGI_HPP
#define CGI_HPP

#include "Client.hpp"
#include "Method.hpp"
#include "RequestHeader.hpp"
#include "ResponseHeader.hpp"
#include "Server.hpp"


#define FD_READ  0
#define FD_WRITE 1

#define PS_CHILD 0

class Client;
class KQueue;
class LocationBlock;


class Cgi : public Method
{
 public:
  Cgi(KQueue &kq, const std::string &path, Client *client);
  virtual ~Cgi();

  int   EventRead();
  int   EventWrite();
  int   EventWriteToCgi();
  void  SetResponseMessage();

  int fromCgi[2];
  int toCgi[2];

 private:
  Cgi();
  Cgi(const Cgi &);
  Cgi &operator=(const Cgi &);

  std::string data_;
};

#endif
