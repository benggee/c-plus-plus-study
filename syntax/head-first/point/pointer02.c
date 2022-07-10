#include <stdio.h>


int main()
{
	int contestants[] = {1,2,3};

	int *choice = contestants;
	contestants[0] = 2;
	contestants[1] = contestants[2];
	contestants[2] = *choice;

	printf("My Choice is:%i\n", contestants[2]);

	printf("Arr Addr:%p\n", contestants);

	printf("Arr Contente: %i\n", contestants);

	printf("Arr Addr Addr: %p\n", &contestants);

	printf("Arr Addr2:%p\n", &choice);

	return 0;
}
