#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


void *thmain(void *arg);
// 线程清理函数
void thclean(void *arg);

int main(int argc, char *argv[]) {
	pthread_t ptid1;
	pthread_t ptid2;
	pthread_t ptid3;
	if (pthread_create(&ptid1, NULL, thmain, NULL) != 0) {
		printf("pthrad create failed.\n");
		return -1;
	}
	if (pthread_create(&ptid3, NULL, thmain, NULL) != 0) {
		printf("pthread create failed.\n");
		return -1;
	}

	// 线程分离
	// 线程分离后，不能通过pthread_join到结果
	pthread_detach(ptid1);
	
	// 使用参数的方式分离
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // 设置线程的属性

	if (pthread_create(&ptid2, &attr, thmain, NULL) != 0) {
		printf("pthread create failed.\n");
		return -1;
	}

	pthread_attr_destroy(&attr);


	// 由于上面分离了线程，这里是拿不到线程返回结果的。
	void *pv = 0;
	pthread_join(ptid1, &pv);
	printf("pthread_join, ptid=%ld, ret=%d\n", ptid1, (int)(long) pv);

	void *pv1 = 0;
	pthread_join(ptid2, &pv1);
	printf("pthread_join, ptid=%ld, ret=%d\n", ptid2, (int)(long) pv1);

	pthread_join(ptid3, NULL);
	printf("pthread_join, ptid3\n");

	return 0;
}

void *thmain(void *arg) {
	// pthread_self() 获取当前线程id
	
	printf("pthread ptid(%ld)\n", pthread_self());

	pthread_cleanup_push(thclean, NULL);
	printf("th main...\n");

	pthread_cleanup_pop(1);
}

void thclean(void *arg) {
	printf("this is thread clean task.\n");
}
