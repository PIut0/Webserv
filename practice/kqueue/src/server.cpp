#include "server.hpp"
#include "util.hpp"

Server::Server(KQueue &_kq, int port) : kq(_kq)
{
	if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		exit_with_perror("socket");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		exit_with_perror("bind");

	if (listen(server_socket, 64) == -1)
		exit_with_perror("listen");

	fcntl(server_socket, F_SETFL, O_NONBLOCK);
}

Server::~Server()
{
	close(server_socket);
}

int Server::event_read()
{
	int client_socket;
	if ((client_socket = accept(server_socket, NULL, NULL)) == -1)
		exit_with_perror("server_event_read");
	new Client(kq, client_socket);

	return 1;
}
