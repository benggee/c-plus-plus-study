#include "_public.h"

#define MAXNUMP_	1000	// 最大进程数
#define SHMKEYP_	0x5095	// 共享内存的key
#define	SEMKEYP_	0x5095	// 信号量的key

// 进程心跳信息结构体
struct st_pinfo {
	int		pid; 
	char 	pname[5];
	int 	timeout;
	time_t	atime;
};

class KeepAlive {
	CSEM	m_sem;		// 信号量，用于加锁
	int		m_shmid;	// 共享内存的id
	int 	m_pos;		// 当前进程在共享内存进程组中的位置
	struct 	st_pinfo *m_shm;	// 指向共享内存的地址空间
public:
	KeepAlive();
	bool AddPInfo(const int timeout, const char *pname);
	bool UptATime();
	~KeepAlive();
};
