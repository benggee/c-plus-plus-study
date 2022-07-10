#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <signal.h>

// 条件变量
// pthread_cond_t cond;
// int pthread_cond_init()
// int pthread_cond_destroy()
// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//
// int pthread_cond_wait()
// int pthread_cond_timedwait()
//
// int pthread_cond_signal()	// 唤醒一个待待中的线程
// int pthread_cond_broadcast() // 唤醒全部等待中的线程
//
// int pthread_condattr_getpshared() // 获取共享属性
// int pthread_condattr_setpshared() // 设置共享属性
// int pthread_condattr_getclock()	 // 获取时钟属性
// int pthread_condattr_setclock()   // 设置时钟属性

pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void handle(int sig);

void *ptmain(void *arg);

int main(int argc, char *argv[]) {
	signal(15, handle);
	
	pthread_t ptid1, ptid2, ptid3;

	if (pthread_create(&ptid1, NULL, ptmain, NULL) != 0) {
		printf("ptid1 create failed.\n");
		return -1;
	}
	if (pthread_create(&ptid2, NULL, ptmain, NULL) != 0) {
		printf("ptid2 create failed.\n");
		return -1;
	}
	if (pthread_create(&ptid3, NULL, ptmain, NULL) != 0) {
		printf("ptid3 create failed.\n");
		return -1;
	}

	pthread_join(ptid1, NULL);
	pthread_join(ptid2, NULL);
	pthread_join(ptid3, NULL);

	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);

	return 0;
}

void *ptmain(void *arg) {
	while(true) {
		printf("线程%lu开始等待条件信号...\n", pthread_self());
		pthread_cond_wait(&cond, &mutex);
		printf("线程%lu开始等待条件信号成功.\n被唤醒后要执行的逻辑.\n\n", pthread_self());
	}
}

void handle(int sig) {
	printf("发送条件信号...\n");
	// pthread_cond_signal(&cond); // 唤醒一个线程
	pthread_cond_broadcast(&cond); // 通知所有等待线程
}




