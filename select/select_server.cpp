#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int initserver(int port);


int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("usage: ./select_server port\n");
		return -1;
	}

	// 初始化服务端用于监听的socket
	int listensock = initserver(atoi(argv[1]));
	printf("listensock=%d\n", listensock);
	
	if (listensock < 0) {
		printf("initserver() failed.\n");
		return -1;
	}

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(listensock, &readfds);

	int maxfd = listensock;

	while(true) {
		// select函数会对传入的集合做修改，为了不影响原有的集合，这里要用一个临时的集合
		fd_set tmpfds = readfds;

		// 超时
		struct timeval timeout;
		timeout.tv_sec = 10; // 秒
		timeout.tv_usec = 0; // 微秒
	
		// 10秒之后会走到超时逻辑
		int sret = select(maxfd+1, &tmpfds, NULL, NULL, &timeout);
		if (sret < 0) {
			printf("select() failed.\n");
			break;
		}

		if (sret == 0) {
			printf("select() timeout.\n");
			continue;
		}

		for (int eventfd = 0; eventfd <= maxfd; eventfd++) {
			if (FD_ISSET(eventfd, &tmpfds) <= 0) {
				continue;
			}
		
			// 如果发生的事件是listensock, 表示有新的客户端连上来
			if (eventfd == listensock) {
				struct sockaddr_in client;
				socklen_t len = sizeof(client);
				int clientsock = accept(listensock, (struct sockaddr *)&client, &len);
				if (clientsock < 0) {
					perror("accept() failed.");
					continue;
				}

				printf("[%ld] accept client(socket=%d) ok.\n", time(0), clientsock);

				// 把新客户端加入到readfds集合
				FD_SET(clientsock, &readfds);
				if (maxfd < clientsock) {
					maxfd = clientsock;
				}
			} else { // 客户端的事件
				char buffer[1024];	
				memset(buffer, 0, sizeof(buffer));
				if (recv(eventfd, buffer, sizeof(buffer), 0) <=0) {
					// 如果客户端断开
					printf("[%ld] client(eventfd=%d) disconnected.\n", time(0), eventfd);
					close(eventfd);
					FD_CLR(eventfd, &readfds);

					// 重新计算maxfd
					if (eventfd == maxfd) {
						for (int i = maxfd; i > 0; i--) {
							if (FD_ISSET(i, &readfds)) {
								maxfd = i;
								break;
							}
						}
					}
				} else {
					// 如果客户端有报文发过来
					// printf("recv(eventfd=%d): %s\n", eventfd, buffer);
					// 把报文原封不动发回去
					send(eventfd, buffer, strlen(buffer), 0);
				}
			}
		}
	}


	return 0;
}


int initserver(int port) {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket() failed.\n");
		return -1;
	}

	int opt = 1;
	unsigned len = sizeof(opt);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, len);

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	// 查看tcp缓冲区大小
	int bufsize = 0;
	socklen_t optlen = sizeof(bufsize);
	getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &bufsize, &optlen); // 获取接收缓冲区大小
	printf("recv bufsize=%d\n", bufsize);
	getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &bufsize, &optlen); // 获取发送缓冲区大小
	printf("send bufsize=%d\n", bufsize);

	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("bind() failed.");
		close(sockfd);
		return -1;
	}

	if (listen(sockfd, 5) != 0) {
		perror("listen() failed.");
		close(sockfd);
		return -1;
	} 

	return sockfd;
}
