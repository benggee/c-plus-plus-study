#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char* argv[])
{
	char *feeds[] = {"https://rsshub.app/jianshu/user/yZq3ZV",
					 "https://rsshub.app/zhihu/hotlist"};

	int times = 3;
	char *phrase = argv[1];
	int i;
	for (i = 0; i < times; i++) {
		char var[255];
		sprintf(var, "RSS_FEED=%s", feeds[i]);
		char *vars[] = {var, NULL};
		pid_t pid = fork();
		if (pid == -1) {
			fprintf(stderr, "Fork faild.");
			return 1;
		}
		if (!pid) { 
			if (execle("/usr/bin/python", "/usr/bin/python", 
						"rssgossip.py", "音乐", NULL,vars)==-1) 
				fprintf(stderr, "Can't run script: %s\n", strerror(errno));
				return 1;
		}
	}
	return 0;
}
