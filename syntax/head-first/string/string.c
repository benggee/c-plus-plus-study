#include <stdio.h>
#include <string.h>


int  main()
{
	char *s1 = "ABCD";
	char *s2 = "ABCD";

	int ret = strcmp(s1,s2);
	printf("Res:%i\n", ret);

	char *p_ret = strstr(s1, "aB");
	printf("Res02:%p\n", p_ret);

	// strstr
	// strcmp
	// strcat
	// strcpy
	// strchz
	// strlen
	
	return 0;
}
