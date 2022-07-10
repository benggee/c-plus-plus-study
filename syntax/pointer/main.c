#include <stdio.h>
#include <stdlib.h>
#include "pointer.h"


int main(int argv, char* argvc[])
{
    // 指针声明
    pointer_statement();

    // 指针的使用
    pointer_use();

    // 查找字符
    char test_str[128] = "This is my c study";
    int ret = find_char(test_str, 'c');
    printf("The char location is:%d\n", ret);

    // 函数指针
    printf("======================= func pointer ==========================\n");
    func_pointer();
}