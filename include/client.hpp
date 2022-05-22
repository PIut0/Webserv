#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "kqueue.hpp"
#include "socket.hpp"
#include "utils.hpp"

#define CRLF "\r\n\r\n"

class KQueue;

class Client : public Socket
{
public:
	std::string req;
	std::string res;
	int has_body;

	Client(KQueue &_kq, int fd);
	~Client();

	int event_read();
	int event_write();

	void parse_req();
};

#endif
