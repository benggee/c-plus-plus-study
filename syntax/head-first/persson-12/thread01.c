#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

void error(char *msg)
{
	fprintf(stderr, "%s: %s", msg, strerror(errno));
	exit(1);
}

pthread_mutex_t a_lock = PTHREAD_MUTEX_INITIALIZER;
int beers = 2000000;
void* drink_lots(void *a)
{
	pthread_mutex_lock(&a_lock);

	int i; 
	for(i = 0; i < 100000; i++)
	{
		beers = beers - 1;
	}

	pthread_mutex_unlock(&a_lock);
	return NULL;
}

int main()
{
	pthread_t threads[20];
	int t;
	printf("%i bottles of beer on the wall \n%i bottles of beer\n", beers, beers);
	for (t = 0; t < 20; t++) {
		pthread_create(&threads[t], NULL, drink_lots, NULL);
	}

	void* result;
	for (t = 0; t < 20; t++) {
		if (pthread_join(threads[t],&result)==-1)
			error("pthread recive error.");
	}

	printf("There are now %i bottles of beer on the wall\n", beers);
	return 0;
}
