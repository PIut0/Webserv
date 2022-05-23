#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

#define PORT 8080
#define IPADDR "127.0.0.1"
#define BUFF_SIZE 1024

int main(int argc, char **argv)
{
	static char buff[BUFF_SIZE];

	int port = argc > 1 ? atoi(argv[1]) : PORT ;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		fprintf(stderr, "socket 생성 실패\n", errno);
		exit(errno);
	}
	printf("port: %d\n", port);

	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(IPADDR);

	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		std::cout << "Can not connect" << std::endl;
		return -1;
	}
	std::cout << "client socket fd : " << sockfd << std::endl;

	while (1)
	{
		std::cout << "input message : ";
		std::cin.getline(buff, BUFF_SIZE);
		if (strncmp(buff, "quit", 4) == 0)
		{
			std::cout << "client quit" << std::endl;
			break;
		}
		else if(strncmp(buff, "!", 1) == 0)
		{
			buff[0] = '\r';
			buff[1] = '\n';
			buff[2] = '\r';
			buff[3] = '\n';
			buff[4] = '\0';
		}
		if (write(sockfd, buff, strlen(buff)) == -1)
			return -1;
		memset(buff, 0, BUFF_SIZE);
	}
	write(sockfd, "\r\n\r\n", 4);

	read(sockfd, buff, BUFF_SIZE);
	printf("recive: %s\n", buff);
	close(sockfd);
	return 0;
}
