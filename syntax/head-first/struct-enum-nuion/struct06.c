#include<stdio.h>


typedef struct {
	const char *name;
	char *name2[100];
	int age;
} peple;

int main()
{
	peple p = {};
	p.name = "aaaa";
	p.name = "bbbb";
	
	printf("This is name:%s\n", p.name);
	printf("This is name2:%s\n", (&p)->name);

	// Err!! p is not a pointer 
	// printf("This is name3:%s\n", p->name);
	
	return 0;
}
