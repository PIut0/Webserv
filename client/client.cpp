#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>

#define IPADDR "127.0.0.1"
#define BUFF_SIZE 10000

int main(int argc, char **argv)
{
  static char buff[BUFF_SIZE];

  if (argc != 3)
  {
    std::cout << "Usage: ./client <port> <path>" << std::endl;
    return 0;
  }

  int port = atoi(argv[1]);
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    fprintf(stderr, "socket 생성 실패\n", errno);
    exit(errno);
  }
  printf("port: %d\n", port);
  printf("path: %s\n", argv[2]);

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

  int file_fd = open(argv[2], O_RDONLY);
  read(file_fd, buff, BUFF_SIZE);

  write(sockfd, buff, strlen(buff));

  sleep(1);
  memset(buff, 0, BUFF_SIZE);
  read(sockfd, buff, BUFF_SIZE);
  printf("recive: %s\n", buff);
  close(sockfd);
  return 0;
}
