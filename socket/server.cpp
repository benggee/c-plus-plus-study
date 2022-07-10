#include "_public.h"

// 父进程退出函数
void FathEXIT(int sig);

// 子进程退出函数
void ChldEXIT(int sig);

// 业务处理主流程
bool _main(const char *strrecvbuffer,char *strsendbuffer);

bool login(const char *strrecvbuffer,char *strsendbuffer);
bool account(const char *strrecvbuffer, char *strsendbuffer);
bool heatbeat(const char *strrecvbuffer, char *strsendbuffer);

bool bsession = false;

CTcpServer TcpServer;

CLogFile logfile;

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Using: ./server port logfile timeout\n Example:./server 5005 /tmp/demo/server.log 30\n\n");
		return -1;
	}

	if (logfile.Open(argv[2], "a+") == false) {
		printf("logfile.Open(%s) failed.\n", argv[2]);
		return -1;
	}
	
	CloseIOAndSignal();
	signal(SIGINT, FathEXIT);
	signal(SIGTERM, FathEXIT);

	if (TcpServer.InitServer(atoi(argv[1])) == false) {
		printf("TcpServer.InitServer(%s) failed.\n", argv[1]);
		return -1;
	}

	while(true) {
		if (TcpServer.Accept() == false) {
			printf("TcpServer.Accept() failed.\n");
			return -1;
		}

		printf("客户端(%s)已连接。\n", TcpServer.GetIP());

		// 如果是父进程，则跳到继续跳到accept
		if (fork() > 0) {
			continue;
		}

		TcpServer.CloseListen();

		signal(SIGINT, ChldEXIT);
		signal(SIGTERM, ChldEXIT);

		// 子进程与客户端进行通讯，处理业务
		char strrecvbuffer[1024];
		char strsendbuffer[1024];

		char buffer[102400];
		while(1) {
			memset(strrecvbuffer, 0, sizeof(strrecvbuffer));
			if (TcpServer.Read(strrecvbuffer, atoi(argv[3])) == false) {
				break;
			}
		
			memset(strsendbuffer,0, sizeof(strsendbuffer));
			if (_main(strrecvbuffer, strsendbuffer) == false) {
				break;
			}

			if (TcpServer.Write(strsendbuffer) == false) {
				break;
			}

			printf("发送：%s\n", strsendbuffer);	
		}

		ChldEXIT(0);
	}
}

// 父进程退出函数
void FathEXIT(int sig) {
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	logfile.Write("父进程退出=%s.\n", sig);

	// 关闭监听的socket
	TcpServer.CloseListen();

	// 通知全部子进程退出 
	kill(0, 15);
	
	exit(0);
}

// 子进程退出函数
void ChldEXIT(int sig) {
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	logfile.Write("子进程退出, sig=%d.\n", sig);
	
	// 关闭客户端的socket
	TcpServer.CloseClient();

	exit(0);
}

bool _main(const char *strrecvbuffer,char *strsendbuffer) {
	// 解析strrecvbuffer,获取业务代码
	int isrvcode = -1;
	GetXMLBuffer(strrecvbuffer, "srvcode", &isrvcode);

	if ((isrvcode != 1) && (bsession == false)) {
		strcpy(strsendbuffer, "<retcode>-1</retcode><message>用户未登陆</message>");
		return true;
	}

	// 处理每种业务
	switch(isrvcode) {
		case 0:
			heatbeat(strrecvbuffer, strsendbuffer);
			break;
		case 1:
			login(strrecvbuffer, strsendbuffer);
			break;
		case 2:
			account(strrecvbuffer, strsendbuffer);
			break;
		default:
			logfile.Write("业务代码不合法:%s\n", strrecvbuffer);
			return false;
	}

	return true;
}

bool login(const char *strrecvbuffer, char *strsendbuffer) {
	// 解析strrcvbuffer
	char tel[21];
	char passwd[31];

	GetXMLBuffer(strrecvbuffer, "tel", tel);
	GetXMLBuffer(strrecvbuffer, "passwd", passwd);

	if ((strcmp(tel, "18611112222") == 0) && (strcmp(passwd, "abc123") == 0)) {
		bsession = true;
		strcpy(strsendbuffer, "<retcode>0</retcode><message>成功.</message>");
	} else {
		strcpy(strsendbuffer, "<retcode>-1</retcode><message>失败.</message>");
	}

	return true;
}

bool account(const char *strrecvbuffer, char *strsendbuffer) {
	char cardid[1024];
	GetXMLBuffer(strrecvbuffer, "cardid", cardid);
	
	if (strcmp(cardid, "62200000001") == 0) {
		strcpy(strsendbuffer, "<retcode>0</retcode><message>成功.</message><ye>100.58</ye>");
	} else {
		strcpy(strsendbuffer, "<retcode>-1</retcode><message>失败.</message>");
	}

	return true;
}

bool heatbeat(const char *strrecvbuffer, char *strsendbuffer) {
	strcpy(strsendbuffer, "<retcode>1></retcode><message>心跳success</message");
	return true;
}
