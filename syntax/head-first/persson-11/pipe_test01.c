#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}


int main()
{
	pid_t pid = fork();
	if (pid == -1)
		error("Fork Error.");

	int pf[2];
	if (pipe(pf) == -1) 
		error("pipe create error.");
	if (!pid) { 
		printf("This is child:%i\n",pid);
		
		//close(pf[0]);
		dup2(pf[1],1);
		close(pf[0]);
		system("wget http://www.baidu.com");
		printf("10000\n");
		fprintf(stdout, "20000: %i\n", pid);
	}

	dup2(pf[0], 0);
	close(pf[1]);
	char msg[120]; 
	printf("The pfId:%i\n", pf[0]);
	while(fgets(msg, 120, stdin)) {
		if (msg[0] == '\t')
			printf("This is code:%s\n", msg);
	}

	//printf("This is code:%s\n", msg);
	return 0;

}
