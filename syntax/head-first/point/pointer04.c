#include <stdio.h>


void skip(char *msg) 
{
	puts(msg+6);
}

int main()
{
	char s[] = "This is six char string!";
	skip(s);
	return 0;
}
