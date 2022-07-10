//
// Created by HEADS on 2021/1/30.
//
// 可以命令查看编译器提供的宏定义
// g++ -E -dM - < /dev/null
//
// 可以使用-E参数把预处理的代码加到新的源码文件
// g++ test01.cpp -E -o a.cxx

#if __linux__           // 预处理检查宏是否存在
#define HAS_LINUX 1     // 宏定义
#endif                  // 预处理条件结束
#                       // 预处理空行

// 文件包含
// include可以包含任何文件，不做检查
#include <iostream>
// #include "a.out"

// 解决重复包含的问题
#ifndef _XXX_H_INCLUDED_
#define _XXX_H_INCLUDED_

#include <string.h>

#endif


// 用外部文件替换代码内容
// 注意几个细节：
// 1. 文件名不一定是要.inc
// 2. include行不用分号结尾
static uint32_t calc_table[] = {
#include "calc_values.inc"  // 非常大的一个数组，可以把里面的内容放到外面，简化代码
};


// 宏定义（#define/#undef）
// 注意细节：
// 1. 宏的展开、替换发生在预处理阶段，不涉及函数调用、参数传递、指针寻址，没有任何运行效率损失
// 2. 宏没有作用域的概念，永远是全局的

// 对于频繁调用的小代码片段
#define ngx_tolower(c)    ((c > = 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ngx_toupper(c)    ((c > = 'a' && c <= 'z') ? (c & ~0x20) : c)

#define ngx_memzero(buf, n)   (void) memset(buf, 0, n)

// 避免冲突使用undef取消定义
#define CUBE(a) (a) * (a) * (a)  // 定义一个求立方的宏
// ...
// cout << CUBE(10) << endl;
// cout << CUBE(15) << endl;
// ...
#undef CUBE // 使用完立即取消定义

// 预先检查
#ifdef AUTH_PWD         // 检查是否已经有宏定义
#undef AUTH_PWD         // 如果有就取消定义
#endif
#define AUTH_PWD "xxx"  // 重新定义

// 定义常量
#define MAX_BUF_LEN     65535
#define VERSION         "1.0.18"

// 文件替换
#define BEGIN_NAMESPACE(x) namesapce x {
#define END_NAMESPACE(x) }

BEGIN_NAMESPACE(my_own)
// ... // functions and classess
END_NAMESPACE(my_own)


// 条件编译（#if/#else/#endif）
// __cplusplus 标记了C++语言版本号
#ifdef __cplusplus                      // 定义了这个宏就是在用c++编译器
    extern "C" {                        // 函数按照C的方式去处理
#endif
    void a_c_function(int a);
#ifdef __cplusplus                      // 检查是否是c++编译器
    }                                   // extern "C" 结束
#endif

#if __cplusplus >= 201402               // 检查C++标准的版本号
    cout << "c++14 or later" << endl;   // 201402就是C++14
#elif __cplusplus >= 201103             // 201103是c++11
    cout << "c++11 or before" << endl;  // 199711是c++98
#else                                   // 太低则报错
// error "c++ is too old"
#endif                                  // 预处理结束


// 一些实例
#if defined(__cpp_decltype_auto)   // 检查是否支持decltype(auto)
    cout << "decltype(auto) enable" << endl;
#else
    cout << "decltype(auto) disable" << endl;
#endif // __cpp_decltype_auto

#if __GNUC__ <= 4
    cout << "gcc is too old" << endl;
#else
    cout << "gcc is good enough" << endl;
#endif

#if defined(__SSE4_2__) && defined(__x85_64)
    cout << "we can do more optimization" << endl;
#endif

// Nginx使用shell脚本检测外部环境，生成一个包含若干宏的源码配置文件
#if (NGX_FREEBSD)
#   include <ngx_freebsd.h>
#elif (NGX_LINUX)
#   include <ngx_linux.h>
#elif (NGX_SOLARTS)
#   include <ngx_solaris.h>
#elif (NGX_DARWIN)
#   include <ngx_darwin.h>
#endif

// 使用"#if 1" "#if 0" 来显示启用或者禁用大段代码，要比/*...*/的注释更加安全
#if 0
// ...
#endif

#if 1
// ...
#endif
