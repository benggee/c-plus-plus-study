#include "_ftp.h"

Cftp ftp;

int main() {
	if (ftp.login("192.168.0.224:21", "ftpuser", "abc123") == false) {
		printf("ftp.login(192.168.0.224:21) failed.\n");
		return -1;
	}

	printf("ftp.login(192.168.0.224:21) ok.\n");

	if (ftp.mtime("tmp/tmp.txt") == false) {
		printf("ftp.mtime(/data/tmp/tmp.txt) failed.\n");
		return -1;
	}

	printf("ftp.mtime(/data/tmp/tmp.txt) ok, mtime=%s.\n", ftp.m_mtime);

	if (ftp.size("tmp/tmp.txt") == false) {
		printf("ftp.size(tmp/tmp.txt) failed.\n");
		return -1;
	}
	printf("ftp.size(/tmp/tmp.txt) ok, size=%d\n", ftp.m_size);

	if (ftp.nlist("tmp", "/tmp/ftptest") == false) {
		printf("ftp.nlist(tmp/) failed.\n");
		return -1;
	}
	printf("ftp.nlist(tmp/) ok.\n");

	// 下载
	if (ftp.get("tmp/tmp.txt", "/tmp/ftpdir/tmp.txt.bak", true) == false) {
		printf("ftp.get() failed.\n");
		return -1;
	}
	printf("ftp.get(), ok\n");

	// 上传
	if (ftp.put("/tmp/ftpdir/tmp.txt.bak", "tmp/tmp-upload.txt.bak", true) == false) {
		printf("ftp.put() failed.\n");
		return -1;
	}
	printf("ftp.put() ok. \n");

	ftp.logout();

	return 0;
}
