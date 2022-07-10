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
#include <vector>
#include "../../lib/_public.h"


#define MAXSOCK 1024  

int cmdconnsock; 						// 内网程序与外网程序的控制通道
int clientsocks[MAXSOCK];				// 存放每个socket连接对端的socket的值
int clientatime[MAXSOCK];				// 存放每个socket连接最后一次收发报文的时间

bool loadroute(const char *inifile); 	// 把代理路由参数加载到vroute容器
int connecttodst(const char *ip, const int port);  // 向目标ip 和端口发送socket连接
void EXIT(int sig);

CLogFile logfile;

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("usage: ./reverse-proxy-in logfile ip port\n\n");
		printf("Sample: ./reverse-proxy-in /tmp/reverse-proxy-in.log 192.168.0.224 4000\n\n");

		printf("logfile 程序运行的日志文件.\n");
		printf("ip		外网代理服务端的地址.\n");
		printf("port	外网代理服务端的端口.\n\n\n");
		return -1;
	}
	
	CloseIOAndSignal();
	signal(SIGINT, EXIT);
	signal(SIGTERM, EXIT);

	if (logfile.Open(argv[1], "a+") == false) {
		printf("打开日志文件失败(%s).\n", argv[1]);
		return -1;
	}

	// 建立内网程序与程序的控制通道
	CTcpClient TcpClient;
	if (TcpClient.ConnectToServer(argv[2], atoi(argv[3])) == false) {
		logfile.Write("TcpClient.ConnectToServer(%s, %s) failed.\n", argv[2], argv[3]);
		EXIT(-1);
	}

	cmdconnsock = TcpClient.m_connfd;
	fcntl(cmdconnsock, F_SETFL, fcntl(cmdconnsock, F_GETFD, 0) | O_NONBLOCK);

	logfile.Write("与外部的控制通道已建立(cmdconnsock=%d)\n", cmdconnsock);
	
	int epollfd = epoll_create(1);
	
	// 声明事件的数据结构
	struct epoll_event ev;

	// 为控制通道的socket准备可读事件
	ev.events = EPOLLIN;
	ev.data.fd = cmdconnsock;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, cmdconnsock, &ev);

	// 创建定时器
	int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK|TFD_CLOEXEC); // 创建timerfd

	struct itimerspec timeout;
	memset(&timeout, 0, sizeof(struct itimerspec));
	timeout.it_value.tv_sec = 20;	// 超时时间为20秒
	timeout.it_value.tv_nsec = 0;	
	timerfd_settime(tfd, 0, &timeout, NULL); 	// 设置定时器

	// 为定时器准备事件
	ev.events = EPOLLIN | EPOLLET;  // 读事件，注意，一定要是边缘模式
	ev.data.fd = tfd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, tfd, &ev);

	// 存放epoll返回的事件
	struct epoll_event evs[10];
	while(true) {
		// 等待监视的socket有事件发生
		int infds = epoll_wait(epollfd, evs, 10, -1);
		if (infds < 0) {
			perror("epoll() failed.");
			break;
		}

		// 如果infds>0，表示有事件发生的socket的数量
		// 遍历epoll返回的已发生的事件的数组evs
		for (int i = 0; i < infds; i++) {
			printf("evets=%d, data.fd=%d\n", evs[i].events, evs[i].data.fd);
			
			// 处理定时器代码
			if (evs[i].data.fd == tfd) {
				// 重置定时器
				timerfd_settime(tfd, 0, &timeout, NULL);
				
				for (int j = 0; j < MAXSOCK; j++) {
					// 如果客户端socket空闲的时间超过80秒就关掉它
					if ((clientsocks[j] > 0) && (time(0) - clientatime[j] > 80)) {
						logfile.Write("client(%d, %d) timeout.\n", clientsocks[j], clientsocks[clientsocks[j]]);
						close(clientsocks[j]);
						close(clientsocks[clientsocks[j]]);
						// 把数组中对端的socket置空, 这一行代码和下行一代码顺序不乱
						clientsocks[clientsocks[j]] = 0;
						clientsocks[j] = 0;
					}
				}

				continue;
			}

			// 如果发生事件的是控制通道
			if (evs[i].data.fd == cmdconnsock) {
				// 读取控制报文内容
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				if (recv(cmdconnsock, buffer, 200, 0) <= 0) {
					logfile.Write("与外网的控制通道已断开.\n");
					EXIT(-1);
				}

				// 如果是心跳报文
				if (strcmp(buffer, "<activetest>") == 0) continue;

				// 向外网服务端发起连接请求
				int srcsock = connecttodst(argv[2], atoi(argv[3]));
				if (srcsock < 0) continue;
				if (srcsock >= MAXSOCK) {
					logfile.Write("连接数已超过最大值%d.\n", MAXSOCK);
					close(srcsock);
					continue;
				}

				// 从控制报文内容中获取目标服务地址和端口
				char dstip[11];
				int	 dstport;
				GetXMLBuffer(buffer, "dstip", dstip, 30);
				GetXMLBuffer(buffer, "dstport", &dstport);

				// 向目标服务地址和端口发起socket连接
				int dstsock = connecttodst(dstip, dstport);
				if (dstsock < 0) {
					close(srcsock); 
					continue;
				}
				if (dstsock >= MAXSOCK) {
					logfile.Write("连接数已超过最大值%d\n", MAXSOCK);
					close(srcsock);
					close(dstsock);
					continue;
				}

				// 把内网和外网的socket对连接在一起
				logfile.Write("新建内外网通道(%d, %d) ok\n", srcsock, dstsock);

				// 为新连接的两个sock准备可读事件，并添加到epoll中
				ev.data.fd = srcsock;
				ev.events = EPOLLIN;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, srcsock, &ev);
				
				ev.data.fd = dstsock;
				ev.events = EPOLLIN;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, dstsock, &ev);

				// 更新cleintsocks数组中两端socket的值和活动时间
				clientsocks[srcsock] = dstsock; 
				clientsocks[dstsock] = srcsock;
				clientatime[srcsock] = time(0);
				clientatime[dstsock] = time(0);
				
				continue;
			}

			// 处理客户端socket事件
			char buffer[1024];
			int  buflen;
			
			// 从一端读取数据 
			memset(buffer, 0, sizeof(buffer));

			// 如果连接断开 
			if ((buflen = recv(evs[i].data.fd, buffer, sizeof(buffer), 0)) <= 0) {
				logfile.Write("cleint(%d, %d) disconnected.\n", evs[i].data.fd, clientsocks[evs[i].data.fd]);

				close(evs[i].data.fd);							// 关闭客户端的连接
				close(clientsocks[evs[i].data.fd]);				// 关闭客户端对端的连接
				clientsocks[clientsocks[evs[i].data.fd]] = 0; 	// 这一行代码和下一行代码的顺序不能乱
				clientsocks[evs[i].data.fd] = 0;				// 这一行代码和上一行代码顺序不能乱

				continue;
			}

			// 成功读取到了数据, 把接收到的报文内容原封不动的发给对端
			logfile.Write("from %d to %d, %d bytes.\n", evs[i].data.fd, clientsocks[evs[i].data.fd], buflen);
			send(clientsocks[evs[i].data.fd], buffer, buflen, 0);
		}
	}

	return 0;
}

// 向目标ip端口发起socket连接
int connecttodst(const char *ip, const int port) {
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		return -1;
	}

	struct hostent* h;
	if ((h = gethostbyname(ip)) == 0) {
		close(sockfd);
		return -1;
	}

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);

	fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK);

	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	return sockfd;
}

void EXIT(int sig) {
	logfile.Write("程序退出. sig=%d\n", sig);

	// 关闭内网程序与外网程序的控制通道
	close(cmdconnsock);

	// 关闭全部客户端的socket
	for (int i = 0; i < MAXSOCK; i++) {
		if (clientsocks[i] > 0) {
			close(clientsocks[i]);
		}
	}

	exit(0);
}
