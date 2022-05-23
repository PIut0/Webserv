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

#include "KQueue.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "ServerBlock.hpp"

#define PORT 8080

class Client;
class KQueue;
class ServerBlock;

class Server : public Socket
{
public:
	struct sockaddr_in server_addr;
	ServerBlock &server_block;

	Server(KQueue &_kq, ServerBlock &_sb);
	~Server();

	int event_read();
	int event_write();
};

#endif
