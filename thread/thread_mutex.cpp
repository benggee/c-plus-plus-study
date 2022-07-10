#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

// 互斥锁
// pthread_mutex_t mutex;
// int pthread_mutex_init();
// int pthread_mutex_lock();
// int pthread_mutex_trylock() // 等待并加锁
// int pthread_mutex_timedlock(); // 带超时的加锁
// int pthread_mutex_unlock();
// int pthread_mutex_destroy();

// 初始化锁
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // 这种方式不用再调用pthread_mutex_init(&mutex, NULL)函数

// 互斥锁的属性
// PTHREAD_MUTEX_TIMED_NP 缺省值，普通锁
// PTHREAD_MUTEX_RECURSIVE_NP 嵌套锁，递归锁，允许同一个线程对同一个锁成功获取多次
// 							  如果不是同线程请求，则在加锁线程解锁时重新竞争
// PTHREAD_MUTEX_ADAPTIVE_NP  适应锁，解锁后，请求锁的线程重新竞争

int var = 0;

pthread_mutex_t mutex;

void *ptmain(void *arg);


int main(int argc, char *argv[]) {
	pthread_mutex_init(&mutex, NULL);

	pthread_t ptid1;
	pthread_t ptid2;

	if (pthread_create(&ptid1, NULL, ptmain, NULL) != 0) {
		printf("pthread create failed.\n");
		return -1;
	}
	if (pthread_create(&ptid2, NULL, ptmain, NULL) != 0) {
		printf("pthread create faile.\n");
		return -1;
	}

	pthread_join(ptid1, NULL);
	pthread_join(ptid2, NULL);

	printf("var ret(%d)\n", var);
	
	pthread_mutex_destroy(&mutex);
	return 0;
}

void *ptmain(void *arg) {
	for (int i = 0; i < 1000000; i++) {
		pthread_mutex_lock(&mutex);
		var++;
		pthread_mutex_unlock(&mutex);
	}	
}
