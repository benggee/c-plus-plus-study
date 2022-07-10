#include <iostream>
#include <memory>
#include <assert.h>

using namespace std;

int main() {
	unique_ptr<int> ptr1(new int(10));	// int智能指针
	assert(*ptr1 != 0);					
	assert(ptr1 != nullptr);			

	unique_ptr<string> ptr2(new string("hello"));
	assert(*ptr2 == "hello");
	assert(ptr2->size() == 5);
	// 智能指针以下的操作是不合法的
	// ptr1++
	// ptr1 += 2
	// delete(ptr1)
	// 不初始化直接使用
	// unique_ptr<int> ptr3;
	// *ptr3 = 42;
	//
	// 在C++14中，可以使用make_unique强制初始化
	// auto ptr3 = make_unique<int>(42);
	// assert(ptr3 && *ptr3 == 42);
	//
	// auto ptr4 = make_unique<string>("god of war");
	// assert(!ptr4->empty());
	
	// unique_ptr所有权
	// 唯一、不共享
	auto ptr5 = move(ptr1); // ptr1变成空指针
	assert(!ptr1 && ptr2);

	// shared_ptr指针
	shared_ptr<int> sptr1(new int(10));		// int 智能指针
	assert(*ptr1 == 10);					// 可以用*取值

	shared_ptr<string> sptr2(new string("hello")); // string智能指针
	assert(*sptr2 == "hello");

	// c++14
	// auto sptr3 = make_shared<int>(42);	// 工厂函数
	// assert(ptr3 && *ptr3 == 42);
	//
	// auto ptr4 = make_shared<string>("zelda");
	// assert(!ptr4->empty());

	/*auto asptr1 = make_shared<int>(42); 	// 工厂函数创建智能指针
	assert(asptr1 && asptr1.unique());

	auto asptr2 = asptr1;
	assert(aspt1 && asptr2);		// 此时两个指针均有效

	assert(!asptr1.unique() && asptr1.use_count() == 2);
	assert(!asptr2.unique() && asptr2.use_count() == 2)
	*/

	// 避免在析构函数运行阻塞任务，如下;
	class DemoShared final
	{
	public:
		DemoShared() = default;
		~DemoShared()
		{
			// stop the world ...
		}
	};

	// 循环引用
	class Node final
	{
	public:
		using this_type		= Node;
		using shared_type	= std::shared_ptr<this_type>;
	public:
		shared_type 		next;	// 使用智能指针来指向下一个节点
	};
	/*
	auto n1 = make_shared<Node>;	
	auto n2 = make_shared<Node>;

	assert(n1.use_count() == 1);
	assert(n2.use_count() == 1);

	n1->next = n2;
	n2->next = n1;

	assert(n1.use_count() == 2); // 引用计数为2
	assert(n2.use_count() == 2); // 无法减到0，无法销毁，导致内存泄露
	*/

	// 打破循环引用
	// weak_ptr
	class Node1 final
	{
	public:
		using this_type 	= Node1;
		using shared_type	= std::weak_ptr<this_type>;
	public:
		shared_type		next;	
	};
	
	/*
	auto nn1 = make_shared<Node1>();
	auto nn2 = make_shared<Node2>();

	nn1->next = nn2;
	nn2->next = nn1;

	assert(nn1.use_count() == 1);		// 因为使用了weak_ptr,引用计数为1
	assert(nn2.use_count() == 1); 		// 打破循环引用，不会导致内存泄露

	// 检查指针是否有效
	if (!nn1->next.expired()) {
		auto ptr = nn1->next.lock(); // lock()获取shared_ptr
		assert(ptr == n2);
	}
	*/

	return 0;
}
