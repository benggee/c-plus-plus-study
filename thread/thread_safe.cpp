#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <atomic>

using namespace std;

void *thmain(void *arg);


// int var = 0;

// c++ atomic的方式
std::atomic<int> var;

// 原子操作函数
// type __sync_fetch_and_add(type *ptr, type value);
// type __sync_fetch_and_sup(type *ptr, type value); // i--
// type __sync_fetch_and_or(type *ptr, type value);  // or
// type __sync_fetch_and_and(type *ptr, type value); // and
// type __sync_fetch_and_nand(type *ptr, type value);// not and, (not a or b)
// type __sync_fetch_and_xor(type *ptr, type value); // xor

int main(int argc, char *argv[]) {
	pthread_t ptid1 = 0;
	pthread_t ptid2 = 0;

	if (pthread_create(&ptid1, NULL, thmain, NULL) != 0) {
		printf("pthread create failed.\n");
		return -1;
	}
	if (pthread_create(&ptid2, NULL, thmain, NULL) != 0) {
		printf("pthread create faile.\n");
		return -1;
	}

	pthread_join(ptid1, NULL);
	pthread_join(ptid2, NULL);

	//printf("var ret(%d)\n", var);
	cout << "var=" << var << endl;
}

void *thmain(void *arg) {
	for (int i = 0; i < 1000000; i++) {
		//__sync_fetch_and_add(&var, 1);
		var++;
	}
}


