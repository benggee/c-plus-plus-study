//
// Created by HEADS on 2021/1/30.
//

#include "string_test.h"
#include <iostream>
#include <string>

using namespace std;


int main() {
    // 定义字符串数组
    char str[11] = "helloworld";
    cout << str << endl;

    char str1[11] = {"helloworld"};
    cout << str1 << endl;

    // 定义一个字符串指针
    char* str2 = "helloworld";
    cout << str2 << endl;

    // str = str2 这种赋值是不合法的
    // 面相反的，str2 = str 是合法的
    str2 = str;
    cout << str2 << endl;

    // 两种方式都可以使用下标的方式访问某个字符
    cout << str[1] << endl;
    cout << str2[1] << endl;

    // 修改字符串
    str2 = "helloworld001";
    cout << str2 << endl;

    // 字符数组可以使用下标修改
    str[2] = 'z';
    cout << str << endl;

    // 字符串长度
    cout << "len:" << strlen(str1) << endl;
    // 字符串占用空间
    cout << "size:" << sizeof(str1) << endl;


    // c++中的string
    string s1;
    string s2 = "helloworld";
    string s3 = ("helloworld");
    string s4 = string("helloworld");


    cout << "======================" << endl;

    // 获取长度
    cout << s2.length() << endl;
    cout << s2.size() << endl;
    cout << s2.capacity() << endl;

    // 字符串比较
    cout << (s2 == s3) << endl;
    cout << (s2 != s3) << endl;

    // 转成c风格的字符串
    const char *c_str = s2.c_str();
    cout << "c style str:" << c_str << endl;
    // 随机访问
    for (int i = 0; i < s2.length(); i++) {
        cout << c_str[i] << endl;
    }

    for (int i = 0; i < s2.length(); i++) {
        cout << s2[i] << endl;
    }

    // 字符串拷贝
    s1 = s2;

    // 字符串连接
    s3 = s2 + s1;
    s3 += s1;

    return 0;
}
