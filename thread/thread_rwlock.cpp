#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

// 读写锁
// pthread_rwlock_t rwlock;
// int pthread_rwlock_init();
// int pthread_rwlock_destroy();
// pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
//
// int pthread_rwlock_rdlock()  
// int pthread_rwlock_tryrdlock();
// int pthread_rwlock_timedrdlock()  // 申请读锁，带超时
//
// int pthread_rwlock_wrlock()
// int pthread_rwlock_trywrlock()
// int pthread_rwlock_timedwrlock()
//
// int pthread_rwlock_unlock()
//
// int pthread_rwlockattr_getpshared()
// int pthread_rwlockattr_setpshared()
// PTHREAD_PROXESS_PRIVATE / PTHREAD_PROCESS_SHARED

int var = 0;

pthread_rwlock_t rwlock =  PTHREAD_RWLOCK_INITIALIZER;

void *ptmain(void *arg);
void handle(int sig);


int main(int argc, char *argv[]) {
	signal(15, handle);

	pthread_t ptid1;
	pthread_t ptid2;
	pthread_t ptid3;

	if (pthread_create(&ptid1, NULL, ptmain, NULL) != 0) {
		printf("pthread create failed.\n");
		return -1;
	}
	if (pthread_create(&ptid2, NULL, ptmain, NULL) != 0) {
		printf("pthread create faile.\n");
		return -1;
	}
	if (pthread_create(&ptid3, NULL, ptmain, NULL) != 0) {
		printf("pthread create failed.\n");
		return -1;
	}

	pthread_join(ptid1, NULL);
	pthread_join(ptid2, NULL);
	pthread_join(ptid3, NULL);
	printf("var ret(%d)\n", var);
	
	pthread_rwlock_destroy(&rwlock);
	return 0;
}

void *ptmain(void *arg) {
	for (int i = 0; i < 100; i++) {
		printf("线程%lu开始申请读锁...\n", pthread_self());
		pthread_rwlock_rdlock(&rwlock);
		printf("线程%lu申请读锁成功.\n\n", pthread_self());
		sleep(5);
		pthread_rwlock_unlock(&rwlock);
		printf("线程%lu已释放读锁.\n\n", pthread_self());
		
		// sleep一会儿，让写锁有机会申请到
		sleep(1);
	}	
}

void handle(int sig) {
	printf("开始申请写锁...\n");
	pthread_rwlock_wrlock(&rwlock);
	printf("申请写锁成功.\n\n");

	sleep(10);

	pthread_rwlock_unlock(&rwlock);
}
