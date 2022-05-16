#include "client.hpp"
#include "util.hpp"

Client::Client(KQueue &_kq, int fd) : kq(_kq), client_socket(fd), read_more(0)
{
	fcntl(client_socket, F_SETFL, O_NONBLOCK);
	kq.add_event(client_socket, EVFILT_READ, this);
}

Client::~Client()
{
	close(client_socket);
}

int Client::event_read()
{
	char buf[1025];
	int n = read(client_socket, buf, sizeof(buf) - 1);
	if (n <= 0)
		return n;
	buf[n] = '\0';
	req += buf;

	std::cout << "====== " << "fd: " << client_socket <<  " buffer ======" << std::endl;
	std::cout << req << std::endl;
	if (req.find(END_OF_REQ) != std::string::npos) {
		parse_req();
		kq.add_event(client_socket, EVFILT_WRITE, this);
	}

	return n;
}

int Client::event_write()
{
	int n = write(client_socket, res.c_str(), res.size());
	if (n <= 0)
		return n;
	res = res.substr(n);
	n = res.size();

	return n;
}

// TODO : parse http req
void Client::parse_req()
{
	std::string tmp = req.substr(req.find(END_OF_REQ) + 4);
	req = req.substr(0, req.find(END_OF_REQ));
	if (req.find("body") != std::string::npos)
		read_more = 1;
	else
		read_more = 0;

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
