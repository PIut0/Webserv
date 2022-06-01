#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "KQueue.hpp"
#include "FdInterface.hpp"
#include "utils.hpp"
#include "RequestHeader.hpp"

class KQueue;

class Client : public FdInterface
{
 private:
  Client();
  Client(const Client &);
  Client &operator=(const Client &);
 public:
  Client(KQueue &kq, int fd);
  ~Client();

  int EventRead();
  int EventWrite();

  int CheckCgi();
  FdInterfaceType ParseHeader(std::string &request_message);
  FdInterfaceType ParseBody(std::string &request_message);
  FdInterfaceType ParseReq();
  const std::string GetFilePath() const;

  RequestHeader *request;
  std::string request_message;
  std::string response_message;
};

#endif
