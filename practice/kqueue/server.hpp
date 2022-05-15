#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include "kqueue.hpp"
#include "client.hpp"
#include "util.hpp"

#define PORT 8080

class Client;
class KQueue;

class Server
{
public:
	KQueue &kq;
	int server_socket;
	struct sockaddr_in server_addr;

	Server(KQueue &_kq, int port);
	~Server();

	int event_read();
};

#endif