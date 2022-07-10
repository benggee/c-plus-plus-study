#include <stdio.h>


int main()
{
	char s[] = "This is a string!";
	char *t = s;

	printf("S addr is:%p\n", s);
	printf("t addr is:%p\n", t);
	printf("&s addr is:%p\n", &s);

	int drinks[] = {2,4,6};
	printf("first: %i\n", drinks[0]);
	printf("first02: %i\n", *drinks);
	printf("second: %i\n", *(drinks+1));
	printf("third: %i\n", *(drinks+2));
	return 0;
}
