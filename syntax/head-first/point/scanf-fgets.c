#include <stdio.h>

int main()
{
	/*char name[4];
	printf("Enter your name:");
	
	scanf("%3s", name);

	printf("Name is:%s\n", name);
	*/

	/*char name1[4];
	char name2[4];
	puts("Type your name:");
	scanf("%3s %3s", name1, name2);

	printf("name1:%s, name2:%s\n", name1, name2);
    */

	char name3[4];
	char name4[10];
	puts("Type your name:");
	fgets(name3, sizeof(name3), stdin);

	printf("name3 is:%s\n", name3);	


	
	return 0;
}
