#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct st_pid {
	int pid;
	char name[51];
};

// ipcs -m  查看共享内存


int main(int argc, char *argv[]) {
	// 共享内存标志
	int shmid;

	// 获取或者创建共享内存，健值为0x5005
	if ((shmid=shmget(0x5005, sizeof(struct st_pid), 0640|IPC_CREAT)) == -1) {
		printf("shmget(0x5005) failed\n");
		return -1;
	}

	// 用于指向共享内存的结构体变量
	struct st_pid *stpid = 0;
	
	// 把共享内存连接到当前进程的地址空间
	if ((stpid=(struct st_pid *)shmat(shmid, 0, 0)) == (void *)-1) {
		printf("shmat failed\n");
		return -1;
	}

	// 共享内存的使用
	printf("pid=%d, name=%s\n", stpid->pid, stpid->name);

	stpid->pid = getpid();
	strcpy(stpid->name, argv[1]);

	printf("pid=%d, name=%s\n", stpid->pid, stpid->name);

	// 把共享内存从当前进程中分离
	shmdt(stpid);
	
	if (shmctl(shmid, IPC_RMID, 0) == -1) {
		printf("shmctl failed\n");
		return -1;
	}

	return 0;
}
