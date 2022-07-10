#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void error(char *msg)
{
	fprintf(stderr,"%s: %s\n", msg, strerror(errno));
	exit(1);
}

void* does_not(void *a)
{
	int i = 0;
	for (i=0; i < 5; i++)
	{
		sleep(1);
		puts("Dies not!");
	}

	return NULL;
}

void* does_too(void *a)
{
	int i = 0;
	for (i=0; i < 5; i++) {
		sleep(1);
		puts("Does too!");
	}
	return NULL;
}

int main()
{
	pthread_t t0;
	pthread_t t1;

	if (pthread_create(&t0, NULL, does_not, NULL)==-1)
		error("Thread create faild.");
	if (pthread_create(&t1, NULL, does_too, NULL)==-1) 
		error("Thread1 create faild.");
	
	void *result;
	if (pthread_join(t0, &result)==-1)
		error("t0 recive faild.");
	if (pthread_join(t1, &result)==-1)
		error("t1 recive faild.");

	return 0;
}
