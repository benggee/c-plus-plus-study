#include <iostream>

using namespace std;

auto func = [](int x) 		// 定义一个lambda表达式
{	
	cout << x * x << endl;  // lambda表达式的具体内容
};

int main() {
	func(3);		// 调用lambda表达式	
}
