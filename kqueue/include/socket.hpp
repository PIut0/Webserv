#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "kqueue.hpp"
#include "util.hpp"

class KQueue;

class Socket
{
public:
	KQueue &kq;
	int socket_fd;

	Socket(KQueue &_kq) : kq(_kq) {};
	Socket(KQueue &_kq, int fd) : kq(_kq), socket_fd(fd) {};
	virtual ~Socket() {};

	virtual int event_read() = 0;
	virtual int event_write() = 0;
};

#endif
