#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

// 自旋锁
// pthread_spinlock_t pinlock;
// int pthread_spin_init();
// int pthread_spin_lock();
// int pthread_spin_trylock();
// int pthread_spin_unlock();
// int pthread_spin_destroy();

int var = 0;

pthread_spinlock_t spinlock;

void *ptmain(void *arg);


int main(int argc, char *argv[]) {
	pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);

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
	
	pthread_spin_destroy(&spinlock);
	return 0;
}

void *ptmain(void *arg) {
	for (int i = 0; i < 1000000; i++) {
		pthread_spin_lock(&spinlock);
		var++;
		pthread_spin_unlock(&spinlock);
	}	
}
