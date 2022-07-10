#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// 信号量
// 一个整数计数器，其数值用于表示空闲临界资源的数量
// 申请资源时，信号量减少，表示可用资源数减少
// 释放资源时，信号量增加，表示可用资源数增加
//
// sem_t *sem;
// int sem_init()  	// 初始化信号量
// int sem_destroy()// 销毁信号量
//
// int sem_wait(sem_t *sem)      信号量的P操作
// int sem_trywait(sem_t *sem)   信号量的P操作, 不阻塞
// int sem_timedwait();			 信号量的P操作, 带超时机制
//
// int sem_post(sem_t *sem); 	 信号量的V操作
//
// int sem_getvalue() 			 获取信号量的值


void *ptmain(void *arg);
void handle(int sig);

sem_t sem;

int var;

int main(int argc, char *argv[]) {
	sem_init(&sem, 0, 1);

	pthread_t ptid1, ptid2;
	
	if (pthread_create(&ptid1, NULL, ptmain, NULL) != 0) {
		printf("pthread create ptid1 failed.\n");
		return -1;
	}
	if (pthread_create(&ptid2, NULL, ptmain, NULL) != 0) {
		printf("pthread create ptid2 failed.\n");
		return -1;
	}

	pthread_join(ptid1, NULL);
	pthread_join(ptid2, NULL);

	sem_destroy(&sem);

	printf("var ret(%d)\n", var);

	return 0;
}

void *ptmain(void *arg) {
	for (int i = 0; i < 1000000; i++) {
		sem_wait(&sem);
		var++;
		sem_post(&sem);
	}
}
