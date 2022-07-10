#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>


void error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

int main()
{
	pid_t di = fork();
	if (di == -1)
		error("Fork Faild.");	
	if (!di) {
		printf("This child proccess: %i\n", di);
		sleep(10);
	}
	
	int pid_status;
	if (waitpid(di, &pid_status, 0) == -1) {
		error("Wait child proccess Error.");
	}
	if (WEXITSTATUS(pid_status))
		puts("Error status non-zero");

	if (!di)
		printf("This wait id:%i Status:%i\n", di, pid_status);

	return 0;
}
