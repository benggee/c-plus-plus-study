#include "keepalive.h"

KeepAlive::KeepAlive() {
	m_shmid = -1; // 共享内存的id
	m_pos	= -1; // 当前进程在共享内存进程组中的位置
	m_shm	= 0;  // 指向共享内存的地址空间
}

bool KeepAlive::AddPInfo(const int timeout, const char *pname) {
	if ((m_shmid = shmget(SHMKEYP_, MAXNUMP_*sizeof(struct st_pinfo), 0640|IPC_CREAT)) == -1) {
		printf("shmget(%x) failed\n", MAXNUMP_);
		return false;
	}
	
	if ((m_sem.init(SEMKEYP_)) == false) {
		printf("m_sem.init(%x) falied\n", SEMKEYP_);
		return false;
	}

	// 将共享内存连接到当前进程空间
	m_shm = (struct st_pinfo *)shmat(m_shmid, 0, 0);

	// 创建当前进程心跳信息结构体变量, 把进程的信息填进去
	struct st_pinfo stpinfo;

	memset(&stpinfo, 0, sizeof(struct st_pinfo));
	stpinfo.pid = getpid();
	strncpy(stpinfo.pname, pname, sizeof(stpinfo.pname));
	stpinfo.timeout = 30;
	stpinfo.atime   = time(0);

	// 进程ID是循环使用的，如果曾经有一个进程异常退出，没有清理自己的心跳信息
    // 它的进程信息将残留在共享内存中，不七的是，当前进程重用了上述进程的ID
    // 这样就会在共享内存中存在两个进程id相同的记录，守护进程检查到残留进程的
    // 心跳时，会向进程ID发送退出信号，这个信号将误杀当前进程

    // 如果共享内存中存在当前进程编号，一定是其它进程残留的数据，当前进程就重用>该位置
	for (int i = 0; i < MAXNUMP_; i++) {
		if (m_shm[i].pid == stpinfo.pid) {
			m_pos = i;
			break;
		}
	}

	m_sem.P(); 

	// 在共享内存中查找一个空位置，把当前进程的心跳信息填入共享内存中
	if (m_pos == -1) {
		for (int i = 0; i < MAXNUMP_; i++) {
			if (m_shm[i].pid == 0) {
				m_pos = i;
				break;
			}
		}
	}

	if (m_pos == -1) {
		m_sem.V();
		return false;
	}

	memcpy(m_shm + m_pos, &stpinfo, sizeof(struct st_pinfo));
	
	m_sem.V();

	return true;
}

bool KeepAlive::UptATime() {
	if (m_pos != -1)  {
		m_shm[m_pos].atime = time(0);
		return true;
	}
	return false;
}

KeepAlive::~KeepAlive() {
	if (m_pos != -1) {
		memset(m_shm + m_pos, 0, sizeof(struct st_pinfo));
	}
	
	if (m_shm) {
		shmdt(m_shm);
	}
}
