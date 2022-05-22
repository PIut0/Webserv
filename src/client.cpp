#include "client.hpp"
#include "util.hpp"

Client::Client(KQueue &_kq, int fd) : Socket(_kq, fd), has_body(0)
{
	fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	kq.add_event(socket_fd, EVFILT_READ, this);
}

Client::~Client()
{
	close(socket_fd);
}

int Client::event_read()
{
	char buf[1025];
	int n = read(socket_fd, buf, sizeof(buf) - 1);
	if (n <= 0)	// n == 0: 클라이언트에서 close & n == -1: 클라이언트 프로세스가 종료됨
		return n;
	buf[n] = '\0';
	req += buf;

	std::cout << "====== " << "fd: " << socket_fd <<  " buffer ======" << std::endl;
	std::cout << req << std::endl;
	if (req.find(CRLF) != std::string::npos) {
		parse_req();
		kq.add_event(socket_fd, EVFILT_WRITE, this);
	}

	return n;
}

int Client::event_write()
{
	int n = write(socket_fd, res.c_str(), res.size());
	if (n <= 0)
		return n;
	res = res.substr(n);
	n = res.size();

	return n;
}

// TODO : parse http req
void Client::parse_req()
{
	std::string tmp = req.substr(req.find(CRLF) + 4);
	req = req.substr(0, req.find(CRLF));
	has_body = (req.find("Content-Length") != std::string::npos && !has_body) ? 1 : 0;

	if (req.find("/") != std::string::npos) {
		res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 38\r\n\r\n<html><body>Hello World!</body></html>";
	}
	else {
		//res = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 39\r\n\r\n<html><body>404 Not Found</body></html>";
		res = req;
	}

	req = tmp;

	std::cout << "====== res ======" << std::endl << res << std::endl;
}
