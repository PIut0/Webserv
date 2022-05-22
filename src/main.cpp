#include <vector>
#include <string>

#include "ServerManager.hpp"
#include "utils.hpp"
#include "kqueue.hpp"
#include "server.hpp"

int main(int argc, char** argv) {
	ServerManager serverManager(check_arg(argc, argv));
	std::vector<Server *> servers;
	KQueue kq;
	for(size_t i = 0 ; i < serverManager.serverBlock.size() ; i++) {
		servers.push_back(new Server(kq, serverManager.serverBlock[i]));
		kq.add_server(*(servers[i]));
	}

	int status;

	while (1) {
		kq.refresh();
		for (int i = 0; i < kq.event_count; i++) {
			Socket *socket_class = static_cast<Socket *>(kq.events[i].udata);
			if (kq.events[i].filter == EVFILT_READ){
				if ((status = socket_class->event_read()) <= 0
					&& !static_cast<Client *>(socket_class)->has_body) {
					delete socket_class;
					std::cout << "fd: " << kq.events[i].ident << ": delete client" << std::endl;
					close(socket_class->socket_fd);
				}
			}
			else if (kq.events[i].filter == EVFILT_WRITE) {
				if ((socket_class->event_write()) <= 0) {
					kq.delete_event(kq.events[i].ident, kq.events[i].filter);
				}
			}
		}
	}
	return (0);
}
