#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
	int i = 10;
	void *p = 0;

	p = (void *)(long) i;

	printf("p=%p\n", p);

	int j = 0;
	j = (int)(long)p;

	printf("j=%d\n", j);

	return 0;
}
