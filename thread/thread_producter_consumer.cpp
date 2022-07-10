#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <vector>

using namespace std;

// 缓存队列消息的结构体
struct st_message {
	int mesgid;
	char message[1024];
} stmesg;

vector<struct st_message> vcache; 

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 生产者
void producter(int sig);
// 消费者
void *consumer(void *arg);
// 清理函数
void cleanup(void *arg);


int main(int argc, char *argv[]) {
	signal(15, producter);

	// 创建三个消费者线程
	pthread_t ptid1, ptid2, ptid3;
	pthread_create(&ptid1, NULL, consumer, NULL);
	pthread_create(&ptid2, NULL, consumer, NULL);
	pthread_create(&ptid3, NULL, consumer, NULL);

	sleep(2);
	pthread_cancel(ptid1);
	pthread_cancel(ptid2);
	pthread_cancel(ptid3);

	pthread_join(ptid1, NULL);
	pthread_join(ptid2, NULL);
	pthread_join(ptid3, NULL);

	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);

	return 0;
}

void producter(int sig) {
	static int msgid = 1;  // 消息计数器
	memset(&stmesg, 0, sizeof(struct st_message));

	pthread_mutex_lock(&mutex); // 给缓存队列加锁

	// 生产数据，放入缓存队列
	stmesg.mesgid = msgid++; 
	vcache.push_back(stmesg);
	stmesg.mesgid = msgid++; 
	vcache.push_back(stmesg);
	stmesg.mesgid = msgid++; 
	vcache.push_back(stmesg);
	stmesg.mesgid = msgid++; 
	vcache.push_back(stmesg);

	pthread_mutex_unlock(&mutex); // 给缓存队列解锁

	pthread_cond_broadcast(&cond); // 发送条件信号, 激活全部线程
}

void *consumer(void *arg) {
	// 注册清理函数
	pthread_cleanup_push(cleanup, NULL);

	struct st_message stmesg;

	while(true) {
		pthread_mutex_lock(&mutex); // 给缓存队列加锁

		// 如果缓存队列为空, 等待，用while防止条件变量虚假唤醒
		while(vcache.size() == 0) {
			pthread_cond_wait(&cond, &mutex);
		}
		
		// 从缓存队列中获取第一条记录, 然后删除该记录
		memcpy(&stmesg, &vcache[0], sizeof(struct st_message)); // 内存拷贝
		vcache.erase(vcache.begin());

		pthread_mutex_unlock(&mutex); 

		printf("phid=%ld, msgid=%d\n", pthread_self(), stmesg.mesgid);
		usleep(100);
	}

	pthread_cleanup_pop(1);
}

void cleanup(void *arg) {
	printf("cleanup ok.\n");

	// 如果要释放所有线程资源，需要在清理函数中释放锁
	pthread_mutex_unlock(&mutex);
}
