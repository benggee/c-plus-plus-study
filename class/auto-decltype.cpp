#include <iostream>
#include <map>
#include <set>

// c++标准特别规定，类的静态成员变量允许使用auto自动推导类型，但为了与非静态成员保持一致
// 还是统一不使用auto比较好
//
// C++14新增了字面量后缀“s”，表示标准字符串，所以就可以用"auto str = "xxx"s"的形式直接
// 推导出std::string类型
//
// c++17为auto增加了一种叫“结构化绑定”的功能，相当于简化了的tie()用法

int main(int argc, char *argv[]) {
	auto i = 0;			// 自动推导为int类型
	auto x = 1.0;		// 自动推导为double类型

	auto str = "hello";	// 自动推导为const char [6]类型

	// 自动推导不出来
	std::map<int, std::string> m = {{1, "a"}, {2, "b"}};	
	
	// 自动推导为map内部的迭代器类型
	auto iter = m.begin();	

	// 自动推导出类型，具体是啥不知道
	auto f = bind1st(std::less<int>(), 2);

	// auto 类型推导只限于 初始化场合
	auto x1 = 0L; 	// 自动推导为long
	auto y = &x;	// 自动推导为long*
	auto z {&x};	// 自动推导为long*
	
	// 这个是错误的用法，没有赋值表达式，不知道是什么类型
	//auto err;
	// auto 不支持类成员变量的自动推导
	

	// auto 总是推导出“值类型”，绝不会是“引用”
	// auto 可以附加上const、volatile、*、&这样的类型修饰符，得到新的类型
	auto&	xj1 = x;		// auto 推导为long, xj1是long&
	auto*	xj2 = &x;	// auto 推导为long, xj2是long*
	const auto& x3 = x;	// auto 推导为long，xj3是const long&
	auto	xj4 = &x3;	// auto 推导为const long*, xj4是const long*


	// decltype
	// decltype不仅能推导出值类型，还能推导出引用类型，也就是表达式的"原始类型"
	int xx = 0;

	decltype(x)		xx1;		// 推导为int, xx1是int
	decltype(x)&	xx2 = x;	// 推导为int, xx2是int&, 引用必须赋值
	decltype(x)*	xx3;		// 推导为int, xx3是int*
	decltype(&x)	xx4;		// 推导为int*, xx4是int*
	decltype(&x)*	xx5;		// 推导为int*, xx5是int**
	decltype(xx2)	xx6 = xx2;// 推导为int&, xx6是int&，引用必须赋值

	// decltype/auto 
	// C++14
	/*int xai = 0;
	decltype(auto)	xa1 = (xai);   	// 推导为int&，因为(expr)是引用类型
	decltype(auto)	xa2 = &xai;		// 推导为int*
	decltype(auto)	xa3 = xa1;		// 推导为int&
	*/

	// c++14 auto可以推导函数返回值
	/*auto get_a_set() 
	{
		std::set<int> s = {1, 2, 3};
		return s;
	}*/

	// decltype的使用场景
	// 1. 函数指针
	void (*signal(int signo, void (*func)(int)))(int);
	using sig_func_ptr_t = decltype(&signal);

	// 类成员自动推导
	class DemoClass final
	{
	public:
		using set_type	= std::set<int>; 	// 集合类型别名
	private:
		set_type	m_set;			// 使用别名定义成员变量
		// 使用decltype计算表达式的类型，定义别名
		using iter_type = decltype(m_set.begin());

		iter_type 	m_pos;	// 类型别名定义成员变量
	};

	return 0;
}
