#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/epoll.h>

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

	// 创建epoll句柄
	int epollfd = epoll_create(1);

	// 为监听的socket准备可读事件
	struct epoll_event ev;	// 声明事件的数据结构
	ev.events = EPOLLIN; 	// 读事件
	ev.data.fd = listensock;// 指定事件的自定义数据，会随着epoll_wait()返回的事件一并返回

	// 把监听的socket的事件加入epollfd中
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listensock, &ev);

	// 存放epoll返回的事件
	struct epoll_event evs[10];

	while(true) {
		// 等待监视的socket有事件发生
		// 超时时间单位是微秒
		int infds = epoll_wait(epollfd, evs, 10, -1);
	
		if (infds < 0) {
			printf("epoll() failed.\n");
			break;
		}

		if (infds == 0) {
			printf("epoll() timeout.\n");
			continue;
		}

		for (int i = 0; i < infds; i++) {
			// 如果发生的事件是listensock, 表示有新的客户端连上来
			if (evs[i].data.fd == listensock) {
				struct sockaddr_in client;
				socklen_t len = sizeof(client);
				int clientsock = accept(listensock, (struct sockaddr *)&client, &len);
				if (clientsock < 0) {
					perror("accept() failed.");
					continue;
				}

				printf("[%ld] accept client(socket=%d) ok.\n", time(0), clientsock);

				// 将客户端添加到epoll中
				ev.data.fd = clientsock;
				ev.events = EPOLLIN;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, clientsock, &ev);
			} else { // 客户端的事件
				char buffer[1024];	
				memset(buffer, 0, sizeof(buffer));
				if (recv(evs[i].data.fd, buffer, sizeof(buffer), 0) <=0) {
					// 如果客户端断开
					printf("[%ld] client(eventfd=%d) disconnected.\n", time(0), evs[i].data.fd);
					// epoll如果客户端关闭，会自动维护事件集合，不用手动清除
					close(evs[i].data.fd);
				} else {
					// 如果客户端有报文发过来
					printf("recv(eventfd=%d): %s\n", evs[i].data.fd, buffer);
					// 把报文原封不动发回去
					send(evs[i].data.fd, buffer, strlen(buffer), 0);
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
