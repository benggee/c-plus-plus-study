#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <semaphore.h>

using namespace std;

// 缓存队列消息的结构体
struct st_message {
	int mesgid;
	char message[1024];
} stmesg;

vector<struct st_message> vcache; 

//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
sem_t notify;
sem_t lock;

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 生产者
void producter(int sig);
// 消费者
void *consumer(void *arg);
// 清理函数
void cleanup(void *arg);


int main(int argc, char *argv[]) {
	signal(15, producter);

	sem_init(&notify, 0, 0); // 用于通知的信号量
	sem_init(&lock, 0, 1);   // 用于加锁, 替代mutex

	// 创建三个消费者线程
	pthread_t ptid1, ptid2, ptid3;
	pthread_create(&ptid1, NULL, consumer, NULL);
	pthread_create(&ptid2, NULL, consumer, NULL);
	pthread_create(&ptid3, NULL, consumer, NULL);

/*	sleep(2);
	pthread_cancel(ptid1);
	pthread_cancel(ptid2);
	pthread_cancel(ptid3);
*/
	pthread_join(ptid1, NULL);
	pthread_join(ptid2, NULL);
	pthread_join(ptid3, NULL);

	// pthread_cond_destroy(&cond);
	sem_destroy(&notify);
	// pthread_mutex_destroy(&mutex);
	sem_destroy(&lock);

	return 0;
}

void producter(int sig) {
	static int msgid = 1;  // 消息计数器
	memset(&stmesg, 0, sizeof(struct st_message));

	//pthread_mutex_lock(&mutex); // 给缓存队列加锁
	sem_wait(&lock);

	// 生产数据，放入缓存队列
	stmesg.mesgid = msgid++; 
	vcache.push_back(stmesg);
	stmesg.mesgid = msgid++; 
	vcache.push_back(stmesg);
	stmesg.mesgid = msgid++; 
	vcache.push_back(stmesg);
	stmesg.mesgid = msgid++; 
	vcache.push_back(stmesg);

	//pthread_mutex_unlock(&mutex); // 给缓存队列解锁
	//
	//要post多次才能让每个线程有执行的机会，
	//关于信号量这块内容还需要再深入理解一下
	sem_post(&lock);
	
	// pthread_cond_broadcast(&cond); // 发送条件信号, 激活全部线程
	sem_post(&notify);
	sem_post(&notify);
	sem_post(&notify);
	sem_post(&notify); 
}

void *consumer(void *arg) {
	// 注册清理函数
	pthread_cleanup_push(cleanup, NULL);

	struct st_message stmesg;

	while(true) {
		//pthread_mutex_lock(&mutex); // 给缓存队列加锁
		sem_wait(&lock);

		// 如果缓存队列为空, 等待，用while防止条件变量虚假唤醒
		while(vcache.size() == 0) {
			//pthread_cond_wait(&cond, &mutex);
			//pthread_mutex_unlock(&mutex);
			sem_post(&lock);
			sem_wait(&notify);
			//pthread_mutex_lock(&mutex);
			sem_wait(&lock);
		}
		
		// 从缓存队列中获取第一条记录, 然后删除该记录
		memcpy(&stmesg, &vcache[0], sizeof(struct st_message)); // 内存拷贝
		vcache.erase(vcache.begin());

		//pthread_mutex_unlock(&mutex); 
		sem_post(&lock);

		printf("phid=%ld, msgid=%d\n", pthread_self(), stmesg.mesgid);
		usleep(100);
	}

	pthread_cleanup_pop(1);
}

void cleanup(void *arg) {
	printf("cleanup ok.\n");
}
