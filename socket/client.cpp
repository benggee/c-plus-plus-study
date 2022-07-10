/*
 * 程序名：demo07.cpp，此程序用于演示采用TcpClient类实现socket通讯的客户端。
 * 作者：吴从周。
*/
#include "_public.h"

// 登陆
bool login(); 

// 帐户信息
bool account();

// 心跳
bool heatbeat();

CTcpClient TcpClient;

int main(int argc,char *argv[])
{
	if (argc!=3)
  	{
    	printf("Using:./demo07 ip port\nExample:./demo07 127.0.0.1 5005\n\n"); return -1;
  	}

  	// 向服务端发起连接请求。
  	if (TcpClient.ConnectToServer(argv[1],atoi(argv[2]))==false)
  	{
    	printf("TcpClient.ConnectToServer(%s,%s) failed.\n",argv[1],argv[2]); return -1;
  	}


	if (login() == false) {
		printf("login() failed.\n");
		return -1;
	} 

	heatbeat();

	if (account() == false) {
		printf("account() failed.\n");
		return -1;
	}
	

	for (int i = 0; i < 5; i++) {
		heatbeat();
	}

	sleep(10);

	if (account() == false) {
		printf("account() falid.\n");
		return -1;
	}

	return 0;
}

// 登陆
bool login() {
	char buf[1024];
	SPRINTF(buf, sizeof(buf), "<srvcode>1</srvcode><tel>18611112222</tel><passwd>abc123</passwd>");
	
	printf("发送：%s\n", buf);
	
	if (TcpClient.Write(buf) == false) {
		return false;
	}

	memset(buf, 9, sizeof(buf));
	if (TcpClient.Read(buf) == false) {
		return false;
	}

	printf("接收:%s\n", buf);

	// 解析返回结果
	int code = -1;
	GetXMLBuffer(buf, "retcode", &code);
	if (code != 0) {
		printf("登陆失败.\n");
		return false;
	}

	printf("登陆成功。\n");

	return true;
}

// 帐户信息
bool account() {
	char buf[1024];

	SPRINTF(buf, sizeof(buf), "<srvcode>2</srvcode><cardid>62200000001</cardid>");
	printf("发送：%s\n", buf);

	if (TcpClient.Write(buf) == false) {
		return false;
	}
	
	memset(buf, 0, sizeof(buf));
	if (TcpClient.Read(buf) == false) {
		return false;
	}

	// 解析返回code
	int code = -1;
	GetXMLBuffer(buf, "retcode", &code);
	if (code != 0) {
		printf("查询余额失败.\n");
		return false;
	}

	printf("ret:%s\n", buf);

	double ye = 0;
	GetXMLBuffer(buf, "ye", &ye);
	
	printf("查询余额成功:%.2f\n", ye);

	return true;
}


bool heatbeat() {
	char buf[1024];
	SPRINTF(buf, sizeof(buf), "<srvcode>0</srvcode>");

	printf("发送心跳：%s\n", buf);
	
	if (TcpClient.Write(buf) == false) {
		return false;
	}

	memset(buf, 0, sizeof(buf));
	if (TcpClient.Read(buf) == false) {
		return false;
	}

	printf("接收服务端对心跳包的回复:%s\n", buf);
	return true;
}
