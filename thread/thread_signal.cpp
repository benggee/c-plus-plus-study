#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

// 多线程信号规则
// 1. 多线程共享信号
// 2. 使用pthread_kill()函数可以发送信号到指定线程
// 3. 任一线程因为信号终止会导致整个进程终止

// 其它函数
// sigaction()
// sigprocmask() 进程信号屏蔽
// pthread_sigmask() 线程信号屏蔽
// sigwait()
// sigwaitinfo()
// sigtimedwait()

void sigfunc(int sig) {
	printf("sigfunc sig(%d)\n", sig);
}

void *thmain(void *arg){
	printf("thmain, ptid(%ld)\n", pthread_self());

	printf("sleep...\n");
	sleep(10);
	printf("sleep done.\n");

	return (void *)1;
}

int main(int argc, char *argv[]) {
	signal(2, sigfunc);
		
	pthread_t ptid;
	
	if (pthread_create(&ptid, NULL, thmain, NULL) != 0) {
		printf("pthread create failed.\n");
		return -1;
	}

	// 如果子线程因为信号终止，则整个进程也会终止
	sleep(2);
	pthread_kill(ptid, 15);

	printf("join...\n");
	pthread_join(ptid, NULL);
	printf("join done.\n");
	
	return 0;
}
