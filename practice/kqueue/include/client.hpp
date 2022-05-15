#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include "kqueue.hpp"
#include "util.hpp"

#define END_OF_REQ "\r\n\r\n"

class KQueue;

class Client
{
public:
	KQueue &kq;
	std::string req;
	std::string res;
	int client_socket;
	int read_more;

	Client(KQueue &_kq, int fd);
	~Client();

	int event_read();
	int event_write();

	void parse_req();
};

#endif
