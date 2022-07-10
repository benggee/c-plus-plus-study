#include <iostream>
#include <string>
#include <set>
#include <vector>

using namespace std;

// 面向对象实现原则
// 1. 尽量少用继承和虚函数（什么是虚函数？？）,
// 2. 一个类尽量只负责单一功能
// 3. 使用final可以禁止继承，比如 class DemoClass final {}
// 4. C++中六大基本函数：
// 	构造函数
// 	析构函数
// 	拷贝构造函数
// 	拷贝赋值函数
// 	转移构造函数（C11） 
// 	转移赋值函数（C11）
// 5. 基本函数的声明方式
// 	DemoClass() = default  	// 明确告诉编译器，使用默认实现
// 	DemoClass() = delete	// 明确禁用某个函数形式，效果是外界无法访问
// 	explicit DemoClass(const string_type& str)	// 显式单参构造函数
// 	explicit operator bool()					// 显式转型为bool

// 面向对象知识扩展
// 1. 如果要写好面向对象的代码，必须要学会设计模式和相关的代码设计原则，比如开闭原则，里氏替换原则
// 2. 在C++里，不要再使用typedef struct {} 的方式来定义结构体，这是传统C的做法。
// 	在C++里应该要使用class来声明类的方式
// 3. C++11新特性move可以消除对象拷贝的成本
// 4. c++11提供一个特殊标识符override，显式地标记虚函数重载
// 5. 推荐将xx.h/xx.cpp写到一个xx.hpp文件


// 委托构造
class DemoDelegating final
{
private:
	int a;		// 成员变量
public:
	DemoDelegating(int x) : a(x)
	{}

	DemoDelegating() : DemoDelegating(0)
	{}

	// 字条串参数构造函数
	// 转换成整数，再委托给第一个构造函数
	DemoDelegating(const string& s): DemoDelegating(stoi(s))  
	{}
};

// 成员变量初始化
class DemoInit final 
{
private:
	int			a = 0; 
	string 		s = "hello";
	vector<int> v{1, 2, 3,};
public:
	DemoInit() = default;
	~DemoInit() = default;
public:
	// 可以单独初始化成员，其他用默认值
	DemoInit(int x) : a(x) {} 
};


// 类型别名
class DemoTypeAliasName final 
{
public:
	using this_type	= DemoTypeAliasName;		// 给自己起别名
//	using kafka_conf_type	= KafkaConfig;		// 给外部类起别名

public:
	using	string_type	=	string;		// 字符串类型别名
	using	uint32_type	= 	uint32_t;		// 整数类型别名

	using	set_type	= set<int>;			// 集合类型别名
	using	vector_type	= vector<string>;	// 容器类型别名

private:
	string_type		m_name	= "tom";		// 使用类型别名声明变量
	uint32_type		m_age	= 23;			// 使用类型别名声明变量
	set_type		m_books;				// 使用类型别名声明变量

private:
	//kafka_conf_type	m_conf;					// 使用类型别名声明变量
};
