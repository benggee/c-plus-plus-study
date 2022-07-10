#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void custom_sig(int sig)
{
	printf("This is custom sig:%s\n", sig);
	exit(1);
}

int catch_sig(int sig, void(*handler)(int))
{
	struct sigaction action;
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	return sigaction(sig, &action, NULL);
}

int main()
{

	pid_t pid = fork();
	if (!pid) {
		printf("This is child.");
		catch_sig(SIGALRM, custom_sig);	
		alarm(5);
	}

	sleep(20);

	return 0;
}
