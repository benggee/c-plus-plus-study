#include <iostream>

using namespace std;
class my_exception: public std::runtime_error
{
public:
	using this_type		= my_exception;			// 给自己起个别名
	using supper_type	= std::runtime_error;	// 给父类也起个别名
public:
	my_exception(const char *msg):	// 构造函数
		supper_type(msg)
	{}

	my_exception() = default;
	~my_exception() = default;
private:
	int code = 0;
};

// 使用函数来抛异常，而不是直接使用throw
[[noreturn]] 					// 属性标签
void raise(const char *msg)		// 函数封装throw，没有返回值		
{
	throw my_exception(msg);	// 抛出异常，也可以有更多逻辑
}

// 保证不抛出异常
// 只是告诉编译器一个承诺，并不是真的不抛出异常
void func_noexcept() noexcept // 声明绝不会抛出异常
{
	cout << "noexcept" << endl;
}

int main() {
	// catch使用const &
	/*try {
		raise("error occured");		// 函数封装throw，抛出异常
	} catch(const exception& e)		// const &捕获异常，可以用基类
	{
		cout << e.what() << endl;	// what()是exception的虚函数
	}*/

	// function-try形式
	// void some_function()
	// try 
	// {
	//	...
	// }
	// catch(...)
	// {
	//	...
	// }

}
