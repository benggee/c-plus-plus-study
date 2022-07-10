#include "../lib/_mysql.h"

int main(int argc, char *argv[]) {
	connection conn;

	// 登陆数据库，返回值：0-成功; 其它是失败,存放了mysql的错误码
	// 失败代码在conn.m_cda.rc中，失败描述在conn.m_cda.message中
	if (conn.connecttodb("127.0.0.1,root,abc123,mysql,3306", "utf8") != 0) {
		printf("connect database failed.\n%s\n", conn.m_cda.message);
		return -1;
	}

	struct st_girls {
		long id;
		char name[11];
		double weight;
		char btime[30];
	} stgirls;

	sqlstatement stmt(&conn);

	stmt.prepare("insert into girls(id, name, weight, btime) values(:1,:2,:3+45,str_to_date(:4,'%%Y-%%m-%%d %%H:%%i:%%s'))");

	// 注意事项：
	// 1. 参数的序号从1开始，连续、递增、参数也可以用？表示。但兼容性不好
	// 2. SQL语句中的右值才能作为参数，表名、字段名、关健字、函数名等都不能作为参数
	// 3. 参数可以参与运算或用于函数的参数
	// 4. 如果SQL语句的主体没有改变，只需要prepare()一次就可以了
	// 5. SQL语句中的每个参数，必须调用bindin()绑定变量的地址
	// 6. 如果SQL语句的主体已经改变，prepare()后，需重新用bindin()绑定变量
	// 7. prepare()方法有返回值，一般不检查，如果SQL语句有问题，调用execute()方法时能发现
	// 8. bindin()方法的返回值固定为0,不用判断返回值
	// 9. prepare()和bindin()之后，每调用一次excute()，就执行一次SQL语句，SQL语句的数据来自被绑定变量的值

	stmt.bindin(1, &stgirls.id);
	stmt.bindin(2, stgirls.name, 30);
	stmt.bindin(3, &stgirls.weight);
	stmt.bindin(4, stgirls.btime, 19);
	

	// 模拟超女数据，向表中插入5条测试数据
	for (int i = 0; i < 5; i++) {
		memset(&stgirls, 0, sizeof(struct st_girls));

		// 为结构体变量的成员赋值
		stgirls.id = i;
		sprintf(stgirls.name, "西施%05dgg", i + 1);
		stgirls.weight = i;
		sprintf(stgirls.btime, "2021-08-25 10:33:%02d", i);

		if (stmt.execute() != 0) {
			printf("stmt.execute() failed.\n%s\n%s\n", stmt.m_sql, stmt.m_cda.message);
			return -1;
		}

		printf("成功插入了%ld条记录.\n", stmt.m_cda.rpc);
	}

	printf("insert data ok.\n");

	conn.commit();

	return 0;
}
