#include "../lib/_mysql.h"

int main(int argc, char *argv[]) {
	connection conn;

	if (conn.connecttodb("127.0.0.1,root,abc123,mysql,3306", "utf-8") != 0) {
		printf("connect database failed.\n%s\n", conn.m_cda.message);
		return -1;
	}

	sqlstatement stmt(&conn);

	int iminid, imaxid;

	stmt.prepare("delete from girls where id>=:1 and id<=:2");
	stmt.bindin(1, &iminid);
	stmt.bindin(2, &imaxid);

	iminid = 1;
	imaxid = 3;

	if (stmt.execute() != 0) {
		printf("stmt.execute() failed.\n%s\n%s\n", stmt.m_sql, stmt.m_cda.message);
		return -1;
	}

	printf("本次删除了girls表%ld条记录.\n", stmt.m_cda.rpc);

	conn.commit();

	return 0;
}
