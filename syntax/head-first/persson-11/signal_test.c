#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void diediedie(int sig)
{
	puts("This is customer gisnal.");
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
	catch_sig(SIGINT,diediedie);
	//sleep(60);
	sleep(5);
	// send customer signal
	raise(SIGINT);
	sleep(10);
	
	return 0;
}

