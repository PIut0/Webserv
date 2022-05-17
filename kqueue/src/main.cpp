#include "kqueue.hpp"
#include "server.hpp"
#include <fcntl.h>

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	KQueue kq;
	Server serv(kq, PORT);
	std::cout << "Server: " << serv.server_socket << std::endl;
	kq.add_server(serv);
	int status;

	while (1) {
		kq.refresh();
		for (int i = 0; i < kq.event_count; i++) {
			if (kq.events[i].filter == EVFILT_READ){
				if (kq.events[i].ident == static_cast<uintptr_t>(serv.server_socket))
					static_cast<Server *>(kq.events[i].udata)->event_read();
				else {
					if ((status = static_cast<Client *>(kq.events[i].udata)->event_read()) <= 0
						&& !static_cast<Client *>(kq.events[i].udata)->read_more) {
						if (status < 0)
							delete static_cast<Client *>(kq.events[i].udata);
						std::cout << "fd: " << kq.events[i].ident << ": delete client" << std::endl;
						close(static_cast<Client *>(kq.events[i].udata)->client_socket);
					}
				}
			}
			else if (kq.events[i].filter == EVFILT_WRITE) {
				if ((static_cast<Client *>(kq.events[i].udata)->event_write()) <= 0) {
					kq.delete_event(kq.events[i].ident, kq.events[i].filter);
				}
			}
		}
	}
}
