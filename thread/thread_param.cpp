#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

void *pfunc1(void *arg);
void *pfunc2(void *arg);
void *pfunc3(void *arg);

typedef struct st_student {
	char name[31];
	int  age;
} ststudent;

int main(int argc, char *argv[]) {
	pthread_t pid1 = 0;
	pthread_t pid2 = 0;
	pthread_t pid3 = 0;
	
	int pa1 = 100;
	if (pthread_create(&pid1, NULL, pfunc1, (void *)(long)pa1) != 0) {
		printf("p1 create failed.\n");
		return -1;
	}

	if (pthread_create(&pid2, NULL, pfunc2, &pa1) != 0) {
		printf("p2 create failed.\n");
		return -1;
	}

	ststudent *st = new ststudent;
	strcpy(st->name, "张三");
	st->age  = 18;
	if (pthread_create(&pid3, NULL, pfunc3, st) != 0) {
		printf("p3 create failed.\n");
		return -1;
	}

	void *pv1 = 0;
	void *pv2 = 0;
	ststudent *pv3 = 0;
	pthread_join(pid1, &pv1);
	pthread_join(pid2, &pv2);
	pthread_join(pid3, (void **)&pv3);

	// 使用return的方式返回
	int a1 = (int)(long)pv1;
	// 使用pthread_exit()返回
	int a2 = (int)(long)pv2;
	// 返回一个结构体

	printf("return, a1=%d\npthread_exit(),a2=%d\nstruct, name=%s;age=%d\n",a1, a2, pv3->name, pv3->age);

	return 0;
}

void *pfunc1(void *arg) {
	// 传值 
	int a = (int)(long)arg;

	printf("pfunc1, arg=%d\n", a);

	int b = a;
	// 使用return的方式返回
	return (void *)(long)b;
}

void *pfunc2(void *arg) {
	// 传地址
	int *a = (int *)arg;
	printf("pfunc1, arg=%d\n", *a);

	// 使用pthread_exit()方式返回
	pthread_exit((void *)2);
}

void *pfunc3(void *arg) {
	// 传结构体
	ststudent *st = (ststudent *)arg;

	printf("pfunc2, st.name=%s, st.age=%d\n", st->name, st->age);

	// 返回一个结构体
	ststudent *st2 = new ststudent;
	st2->age = 100;
	strcpy(st2->name, "张三");

	return (void *)st2;
}
