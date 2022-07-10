/*
 * 程序名：demo10.cpp，此程序演示采用开发框架的CTcpServer类实现socket通讯多进程的服务端。
 * 1）在多进程的服务程序中，如果杀掉一个子进程，和这个子进程通讯的客户端会断开，但是，不
 *    会影响其它的子进程和客户端，也不会影响父进程。
 * 2）如果杀掉父进程，不会影响正在通讯中的子进程，但是，新的客户端无法建立连接。
 * 3）如果用killall+程序名，可以杀掉父进程和全部的子进程。
 *
 * 多进程网络服务端程序退出的三种情况：
 * 1）如果是子进程收到退出信号，该子进程断开与客户端连接的socket，然后退出。
 * 2）如果是父进程收到退出信号，父进程先关闭监听的socket，然后向全部的子进程发出退出信号。
 * 3）如果父子进程都收到退出信号，本质上与第2种情况相同。
 *
*/
#include "../lib/_public.h"

CLogFile logfile;      // 服务程序的运行日志。
CTcpServer TcpServer;  // 创建服务端对象。

void EXIT(int sig);  // 父进程退出函数。
void *worker(void *arg);  // 子线程主函数
void threadclean(void *arg); // 线程清理

// 存放所有线程id
vector<pthread_t> ptids; 

// 用于ptids的自旋锁
pthread_spinlock_t ptidslock; 

int main(int argc,char *argv[])
{
  	if (argc!=3) {
    	printf("Using:./thread_server port logfile\nExample:./thread_server 5005 /tmp/thread_server.log\n\n"); return -1;
  	}

	pthread_spin_init(&ptidslock, 0);

  	// 关闭全部的信号和输入输出。
  	// 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程
  	// 但请不要用 "kill -9 +进程号" 强行终止
  	//
  	// 有计划退出
  	// 1. 设置2和15的信号
  	// 2. 在信号处理函数中取消全部的线程
  	// 3. 在线程清理函数中释放资源
  	CloseIOAndSignal(); signal(SIGINT,EXIT); signal(SIGTERM,EXIT);

  	if (logfile.Open(argv[2],"a+")==false) { printf("logfile.Open(%s) failed.\n",argv[2]); return -1; }

  	// 服务端初始化。
  	if (TcpServer.InitServer(atoi(argv[1]))==false)
  	{
    	logfile.Write("TcpServer.InitServer(%s) failed.\n",argv[1]); return -1;
  	}

  	while (true) {
    	// 等待客户端的连接请求。
    	if (TcpServer.Accept()==false) {
      		logfile.Write("TcpServer.Accept() failed.\n"); EXIT(-1);
    	}

    	logfile.Write("客户端（%s）已连接。\n",TcpServer.GetIP());

  		pthread_t ptid;
		if (pthread_create(&ptid, NULL, worker, (void *)(long)TcpServer.m_connfd) != 0) {
			logfile.Write("pthread_create() failed.\n");
			TcpServer.CloseListen();
			continue;
		}

		// 线程创建成功之后，将线程id保存到容器中
		pthread_spin_lock(&ptidslock);
		ptids.push_back(ptid);
		pthread_spin_unlock(&ptidslock);
  	}
}

void *worker(void *arg) {
   	char buffer[102400];
   	int ibuflen;
   	int connfd = (int)(long) arg;

	// 注册线程清理函数
	pthread_cleanup_push(threadclean, arg);

	// 可以将线程取消设置成立即取消
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	// 分离线程
	pthread_detach(pthread_self());

   	// 与客户端通讯，接收客户端发过来的报文后，回复ok。
   	while (1) {
      	memset(buffer,0,sizeof(buffer));
      	if (TcpRead(connfd, buffer, &ibuflen, 30)==false) break; // 接收客户端的请求报文。
      	logfile.Write("接收：%s\n",buffer);

      	strcpy(buffer,"ok");
      	if (TcpWrite(connfd, buffer)==false) break; // 向客户端发送响应结果。
      	logfile.Write("发送：%s\n",buffer);
   	}

   	// 如果客户端退出，会走到这里
   	// 关闭连接
   	// 从线程集合中删除线程id
   	close(connfd);
 
   	pthread_spin_lock(&ptidslock);
   	for (int i = 0; i < ptids.size(); i++) {
   		if (pthread_equal(pthread_self(), ptids[i])) {
			ptids.erase(ptids.erase(ptids.begin() + i));
			break;
		}
   	}
   	pthread_spin_lock(&ptidslock);

   	pthread_cleanup_pop(1);
}

// 程退出函数。
void EXIT(int sig) {
  	// 以下代码是为了防止信号处理函数在执行的过程中被信号中断。
  	signal(SIGINT,SIG_IGN); signal(SIGTERM,SIG_IGN);

  	logfile.Write("父进程退出，sig=%d。\n",sig);

  	TcpServer.CloseListen();    // 关闭监听的socket。
	
  	// 取消全部线程
	for (int i = 0; i < ptids.size(); i++) {
		pthread_cancel(ptids[i]);
	}

	pthread_spin_destroy(&ptidslock);

	sleep(1);

  	exit(0);
}

void threadclean(void *arg) {
	close((int)(long) arg);
	
	logfile.Write("线程%lu退出.\n", pthread_self());
	printf("线程%lu退出.\n", pthread_self());
}
