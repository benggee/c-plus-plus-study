#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *thmain1(void *arg);
void *thmain2(void *arg);

int num = 0;

// 查看线程的方法：
// ps -Lf pid   
int main(int argc, char *argv[]) {
	pthread_t thid1 = 0;
	pthread_t thid2 = 0;

	if (pthread_create(&thid1, NULL, thmain1, NULL) != 0) {
		printf("pthread_create failed.\n");
		exit(-1);
	}

	if (pthread_create(&thid2, NULL, thmain2, NULL) != 0) {
		printf("pthread_create failed.\n");
	}

	// 线程退出
	// 这里有几个规则：
	// 1. 主线程退出会导致所有子线程立马结束
	// 2. 子线程调用exit()将终止整个进程
	// 3. 缺少行为是终止程序的信号，将导致整个进程终止
	//    子线程如果Core dump整个进程结束
	//
	
	// 取消线程
	sleep(2);
	// pthread_cancel(thid1);

	pthread_join(thid1, NULL);
	pthread_join(thid2, NULL);

	return 0;
}

void func() {
	// 在线程主函数调用其它函数的时候，想要在其它函数里退出线程
	// 就可以使用pthread_exit();
	pthread_exit((void *)1);
}

void *thmain1(void *arg) {
	for (int i = 0; i < 10; i++) {
		num += 1;
		sleep(1);
		printf("ptmain1 sleep(%d) ok.\n", i+1);

		if (i == 2) {
			func();
		}

		if (i > 5) {
			pthread_exit(0);
		}
	}
	
	/*
	这个代码，第二次删除会core dump 从而导致整个进程终止
	int *jj = new int;
	delete jj;
	delete jj;
	*/

	// 线程函数的退出：
	// 1. 直接return
	// return;
	// 2. 使用pthread_exit(0);
	// pthread_exit(0);
}

void *thmain2(void *arg) {
	for (int i = 0; i < 50; i++) {
		sleep(1);
		printf("ptmain2, num(%d)\n", num);
	}
}
