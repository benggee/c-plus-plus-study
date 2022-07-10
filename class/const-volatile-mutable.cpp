#include <iostream>

using namespace std;

int main() {
	// volatile 告诉编译器忽略优化，其修饰的变量会变得不稳定，可以通过下面的方式强行修改
	// 结果最终是2048
	const volatile int MAX_LEN = 1024;
	auto ptr = (int *) (&MAX_LEN);
	*ptr = 2048;
	cout << MAX_LEN << endl; 


	// 如果没有volatile修饰，编译器会自动优化，不能修改成功
	// 结果最终是1024
	const int MAX_LEN2 = 1024;
	auto ptr2 = (int *) (&MAX_LEN2);
	*ptr2 = 2048;
	cout << MAX_LEN2 << endl;

	// volatile会导致变量变得不安全，一般不常使用
	

	// const的使用
	// 1. const可以引用任何类型
	int x = 100;
	const int& rx = x; // 常量引用 
	const int* px = &x;// 常量指针

	// const放在声明的最左边，表示指向常量的指针，指向的内容不允许修改
	string name = "uncharted";
	const string *ps1 = &name;	// 指向常量
	// *ps1 = "spiderman";		// 错误，不允许修改
	
	// const 在*的右边，表示指针不能被修改，但指向的变量可以修改
	string* const ps2 = &name; 	// 指向变量，但指针本身不能修改
	*ps2 = "spiderman";       	// 这样是可以修改的

	// *两边都有const
	const string* const ps3 = &name;  // ????


	// 与类相关的const的用法
	class DemoClass final 
	{
	private:
		const long MAX_SIZE = 256;	// const成员变量
		int		   m_value;			// 成员变量
	public:
		// 对于const成员函数来说
		// 如果类是通过const修饰，那编译器会判断函数是不能操作对象的
		// 这个时候，const成员函数就有意义了。因为这样会让编译器认为const对象的方法是合法的
		int get_value() const		// const成员函数
		{
			return m_value;
		}
	};
	
	// 关键字mutable
	// mutable只能修饰类中的成员变量，表示变量即使在const对象里，也可以修改
	// 在某些特殊场景下，可以将部分成员修改为const函数可以修改
	class DemoClass1 final
	{
	private:
		// mutable mutex_type	m_mutex;	// mutable 成员变量
	public:
		void save_data() const 			// const成员函数
		{
			// 因为mutable修饰了m_mutex，所以在这里可以修改m_mutex
		}
	};

	// volatile:
	// 	1. 禁止编译优化
	// 	2. 影响性能
	// const
	// 	1. 修饰变量，只读
	// 	2. 修饰成员函数，不改变对象状态
	// 	3. 可以被编译器优化
	// 	4. const*常量指针
	// 	5. const&是万能引用
	// mutable
	// 	1. 修饰成员变量
	// 	2. 不影响对象的常量性
	//
	// 其它规则
	// 	1. const_cast是C++的四个转型操作符之一，专门用来去除“常量性”, 可以用在一些极端情况下，最好不用。
	//	2. 成员函数有一个隐含的this参数，所以从主义上来说，const成员函数实际上是传入了一个const this指针
	//	   但因为c++语法限制，无法声明const this，所以就把const 放到了函数后面
	//	3. 依据场景，有的成员函数可能即是const又是非const，所以就会有两种重载形式，比如vector的front(),at()等
	//	   如果是const对象编译器就会调用const版本
	//	4. c++11里mutable又多了一种用法，可以修饰lambda表达式
	//	5. c++11引入了新关键字constexpr，能够表示真正的编译阶段常量，甚至能够编写在编译阶段运行的数值函数。
}

