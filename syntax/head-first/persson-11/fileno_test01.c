#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fd = fopen("./test.txt", "w");

	int fo = fileno(fd);
	printf("The test fileno:%i\n", fo);

	// stdout to test.txt
	dup2(fo,1);

	printf("The print01\n");
	fprintf(stdout, "The print02\n");

	return 0;
}
