#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>


// 使用cond + mutex实现生产消费者模型
// pthread_cond_wait(&cond, &mutex) 逻辑如下：
// 1. 把互斥锁解锁
// 2. 阻塞，等待条件(被唤醒)
// 3. 条件被触发+给互斥锁加锁
//
void *ptmain1(void *arg);
void *ptmain2(void *arg);
void handle(int sig);

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
	pthread_t ptid1, ptid2;
	if (pthread_create(&ptid1, NULL, ptmain1, NULL) != 0) {
		printf("pthread_create failed.\n");
		return -1;
	}
	sleep(1);
	if (pthread_create(&ptid2, NULL, ptmain2, NULL) != 0) {
		printf("pthread_create failed.\n");
		return -1;
	}

	pthread_join(ptid1, NULL);
	pthread_join(ptid2, NULL);

	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);

	return 0;
}

void *ptmain1(void *arg) {
	printf("线程1申请互斥锁...\n");
	pthread_mutex_lock(&mutex);
	printf("线程1申请互斥锁成功.\n");

	printf("线程1开始等待条件信号...\n");
	pthread_cond_wait(&cond, &mutex);
	printf("线程1等待条件信号成功.\n");
}

void *ptmain2(void *arg) {
	printf("线程2申请互斥锁...\n");
	pthread_mutex_lock(&mutex);
	printf("线程2申请互斥锁成功.\n");

	pthread_cond_signal(&cond);

	sleep(5);

	printf("线程2解锁.\n");
	pthread_mutex_unlock(&mutex);

	return 0;

}

/*void *ptmain1(void *arg) {
	printf("线程1申请互斥锁...\n");
	pthread_mutex_lock(&mutex);
	printf("线程1申请互斥锁成功.\n");

	printf("线程1开始等待条件信号...\n");
	pthread_cond_wait(&cond, &mutex);
	printf("线程1等待条件信号成功.\n");
	pthread_mutex_unlock(&mutex);
}

void *ptmain2(void *arg) {
	pthread_cond_signal(&cond);
	sleep(1);

	printf("线程2申请互斥锁...\n");
	pthread_mutex_lock(&mutex);
	printf("线程2申请互斥锁成功.\n");

	printf("线程2开始等待条件信号...\n");
	pthread_cond_wait(&cond, &mutex);
	printf("线程2等待条件信号成功.\n");
}*/
