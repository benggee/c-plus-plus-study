#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

void *thmain(void *arg);

int main(int argc, char *argv[]) {
	pthread_t ptid;
	if (pthread_create(&ptid, NULL, thmain, NULL) != 0) {
		printf("pthread create failed.\n");
		return -1;
	}

	sleep(2);
	pthread_cancel(ptid);

	void *ret;
	pthread_join(ptid, &ret);
	
	printf("pid(%ld), ret(%d)\n", ptid, (int) (long)ret);
	return 0;
}

void *thmain(void *arg) {
	// 线程取消状态和类型只能线程主函数中执行
	
	// 设置线程取消状态
	// pthread_setcancelstate(int state, int *oldstate); 
	// state 表示要设置的取消状态
	// *oldstate 表示保存原来的取消状态
	// state取值：
	// PTHREAD_CANCEL_ENABLE  开启取消
	// PTHREAD_CANCEL_DISABLE 关闭取消，设置之后线程将不能取消
	// 默认取消方式是PTHREAD_CANCEL_ENABLE


	// 设置线程取消类型
	// pthread_setcanceltype(int type, int *oldtype);
	// type取值：
	// PTHREAD_CANCEL_ASYNCHRONOUS	立即取消
	// PTHREAD_CANCEL_DEFERRED		延迟取消，直到下一个取消点，比如close()函数
	// 
	printf("pthread id(%ld)\n", pthread_self());
}
