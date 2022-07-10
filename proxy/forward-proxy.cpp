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

// 代理路由参数结构
struct st_route {
	int		listenport; 	// 本地监听端口
	char	dstip[31];		// 目标主机ip
	int		dstport;		// 目标主机端口
	int 	listensock;		// 本地监听socket
} stroute;

vector<struct st_route> vroute;			// 代理路由的容器
int clientsocks[MAXSOCK];				// 存放每个socket连接对端的socket的值
int clientatime[MAXSOCK];				// 存放每个socket连接最后一次收发报文的时间

bool loadroute(const char *inifile); 	// 把代理路由参数加载到vroute容器
int initserver(int port);
int connecttodst(const char *ip, const int port);  // 向目标ip 和端口发送socket连接
void EXIT(int sig);

CLogFile logfile;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("usage: ./forward-proxy logfile conffile\n\n");
		printf("Sample: ./forward-proxy /tmp/forward-proxy.log ./forward-proxy.conf\n");
		return -1;
	}

	if (logfile.Open(argv[1], "a+") == false) {
		printf("打开日志文件失败(%s).\n", argv[1]);
		return -1;
	}

	// 加载代理参数
	if (loadroute(argv[2]) == false) {
		logfile.Write("加载代理参数失败:%s\n", argv[2]);
		return -1;
	}

	logfile.Write("加载代理参数成功.\n");

	// 初始化服务端用于监听的socket
	for (int i = 0; i < vroute.size(); i++) {
		if ((vroute[i].listensock = initserver(vroute[i].listenport)) < 0) {
			logfile.Write("initserver(%d) failed.\n", vroute[i].listenport);
			return -1;
		}

		// 把监听socket设置成非阻塞
		fcntl(vroute[i].listensock, F_SETFL, fcntl(vroute[i].listensock, F_GETFD, 0) | O_NONBLOCK);
	}
	
	int epollfd = epoll_create(1);
	
	// 声明事件的数据结构
	struct epoll_event ev;

	// 为监听的socket准备可读事件
	for (int i = 0; i < vroute.size(); i++) {
		ev.events = EPOLLIN;	// 读事件
		ev.data.fd = vroute[i].listensock;
		epoll_ctl(epollfd, EPOLL_CTL_ADD, vroute[i].listensock, &ev);
	}

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

			int j = 0;
			// 如果发生事件是listensock，表示有新的客户端连接上来
			for (j = 0; j < vroute.size(); j++) {
				if (evs[i].data.fd == vroute[j].listensock) {
					struct sockaddr_in client;
					socklen_t len = sizeof(client);
					int srcsock = accept(vroute[j].listensock, (struct sockaddr *)&client, &len);
					if (srcsock < 0) break;
					if (srcsock >= MAXSOCK) {
						logfile.Write("连接数已超过最大值%d\n", MAXSOCK);
						close(srcsock);
						break;
					}

					printf("accept client(socket=%d) ok.\n", vroute[j].listensock);

					// 向目标ip和端口发起socket连接
					int dstsock = connecttodst(vroute[j].dstip, vroute[j].dstport);
					if (dstsock < 0) break;
					if (dstsock >= MAXSOCK) {
						logfile.Write("连接数已超过最大值%d\n", MAXSOCK);
						close(srcsock);
						break;
					}

					logfile.Write("accept on port %d client(%d, %d) ok.\n", vroute[j].listenport, srcsock, dstsock);

					// 为新连接的两个socket准备可读事件, 并添加到epoll中
					ev.data.fd = srcsock;
					ev.events = EPOLLIN;
					epoll_ctl(epollfd, EPOLL_CTL_ADD, srcsock, &ev);
					
					ev.data.fd = dstsock;
					ev.events = EPOLLIN;
					epoll_ctl(epollfd, EPOLL_CTL_ADD, dstsock, &ev);

					// 更新clientsock数组中两端socket的值和活动时间
					clientsocks[srcsock] = dstsock;
					clientsocks[dstsock] = srcsock;
					clientatime[srcsock] = time(0);
					clientatime[dstsock] = time(0);

					break;
				}
			}

			// 如果j<vroute.size(), 表示事件在上面的for循环中已被处理
			if (j < vroute.size()) continue;

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

bool loadroute(const char *inifile) {
	CFile File;

	if (File.Open(inifile, "r") == false) {
		logfile.Write("打开代理路由参数文件(%s)失败.\n", inifile);
		return false;
	}

	char strbuf[256];
	CCmdStr CmdStr;

	while(true) {
		memset(strbuf, 0, sizeof(strbuf));

		if (File.FFGETS(strbuf, 200) == false) {
			break;
		}
		char *pos = strstr(strbuf, "#");
		// 删除说明文字
		if (pos != 0) {
			pos[0] = 0;
		}
		DeleteRChar(strbuf, ' ');			// 删除右边的空格
		UpdateStr(strbuf, "  ", " ", true); // 把两个空格替换成一个空格
		CmdStr.SplitToCmd(strbuf, " ");
		if (CmdStr.CmdCount() != 3) {
			continue;
		}

		memset(&stroute, 0, sizeof(struct st_route));
		CmdStr.GetValue(0, &stroute.listenport);
		CmdStr.GetValue(1, stroute.dstip);
		CmdStr.GetValue(2, &stroute.dstport);

		vroute.push_back(stroute);
	}

	return true;
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

	// 关闭全部监听的socket
	for (int i = 0; i < vroute.size(); i++) {
		close(vroute[i].listensock);
	}

	// 关闭全部客户端的socket
	for (int i = 0; i < MAXSOCK; i++) {
		if (clientsocks[i] > 0) {
			close(clientsocks[i]);
		}
	}

	exit(0);
}
