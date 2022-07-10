#include <stdio.h>
#include <stdlib.h>
#include "pointer.h"

// 指针的声明
void pointer_statement()
{
    // 在间接操作指针之前一定要初始化，否则会造成无法预料的结果 
    // NULL表示不指向任何地址的指针
    // NULL指针无法间接访问  对于以下声明，表达式 *p1 = 20; 在大多数系统会报错
    int *p1 = NULL;
    
    int a = 30;
    int *p2 = &a;

    pointer_echo(p2);

    // 指针运算 数组名不能直接参与指针运算
    char var[20] = "Hello world!";
    char *c = var;
    while(*c != '\0') {
        printf("The char is :%c\n",*c);
        c++;
    }

    // 同一行 d是一个整形指针 e是一个整数
    int* d,e;
    // 返回类型是指针的函数
    int *fnn1(); // 先执行() 表示fn函数返回一个int指针
    // 返回一个函数指针
    int (*fnn2)();  // 先执行(*fn)表示是某个类型的指针，再执行() 表示指向一个返回值为int的函数指针
    int *(*fnn3)(); // 表示一个指向返回类型为int指针的函数的指针

    /**
     * int fn()[];  // 这是非法的声明，函数不能返回数组
     * int fn[]();  // 这也是非法的，数组元素是有长度的，但函数的长度不一定是确定的
     */

    // 函数指针数组
    int (*fnn4[5])(); // 表示一个元素为指向返回值为int指针的函数
    int *(*fnn5[5])(); // 表示一个数组元素是一个指针，指针指向一个返回类型为int指针的函数指针数组

    // 在ANSI C要求使用完整的声明，例如：
    int (*fn6[5])(int, float); // 表示一个元素为函数指针的数组，元素指向一个参数为int,float，返回值为int的函数
    int *(*fn7[5])(int, float);

}

// 指针的使用
void pointer_use()
{
    int a = 100;
    int *p1 = &a;
    *p1 = 200;
    printf("a is :%d\n", a);

    // 通过强制类型转换 表示将100地址处的值改为10000,
    // 通常在操作硬件，比如操作网卡的时候才会这么做，在平时使用中不应该这么使用
    // *(int *)100 = 10000;

    // 指针的指针
    int **p2 = &p1;
    printf("the p2 value is:%d\n", **p2);
}

// 指针的输出
void pointer_echo(void *p)
{
    // 输出值
    printf("DATA: %d\n", *(int*)p);
    // 指针强制类型转换
    // (int*)p
    // 转换之后再解引用 
    // *(int*)p
}

// 指针当作参数 
void pointer_param()
{

}

// 指针数组
void pointer_arr()
{
    int *f[5];  // []先执行 表示元素是int型指针的数组
}

// 指针结构
void pointer_struct()
{

}

// 结构指针
void struct_pointer()
{
    
}

// 函数指针
void func_pointer()
{
    // 声明
    int f(int);
    int (*fn)(int) = &f;
    // 使用 
    int ans;
    ans = f(20);
    ans = (*fn)(20);
    ans = fn(20);  // 函数指针的间接访问并不是必需的
    
    // 函数指针 
    int (*fn1[])(int) = {f};
    // 函数指针的使用
    ans = (fn1[0])(3);


    // 转移表
    double a = 100,b = 200,ret;
    // compre_func是一个函数指针数组 
    double (*compre_func[])(double, double) = {add, sub, mul, divv};
    // 调用方法一
    // ret = compre_func[0](a, b);
    // 调用方法二
    ret = (*compre_func[0])(a, b);
    printf("Compre Result:%f\n", ret);
}

// 查找字符
int find_char(char *s, char c)
{
    int num = 1;
    while(*s != '\0') {
        if (*s == c) {
            return num;
        }
        num++;
        s++;
    }
    return 0;
}

double add(double a, double b) 
{
    return a+b;
}
double sub(double a, double b)
{
    return a-b;
}
double mul(double a, double b)
{
    return a/b;
}
double divv(double a, double b)
{
    return a*b;
}
int f(int i){ return 0;}
int fn(int i){return 0;}
int fn1(int i){return 0;}
int *fnn1(int i){ int a=0;int *p=&a; return p;}
int fn2(int i){return 0;}
int fn3(int i){return 0;}
int fn4(int i){return 0;}
int fn5(int i){return 0;}
int fn6(int i){return 0;}
int fn7(int i){return 0;}
int *fnn2(){ int a=0;int *p=&a; return p;}
int *fnn3(){int a=0;int *p=&a; return p;}
int *fnn4(){ int a=0;int *p=&a; return p;}
int *fnn5(){ int a=0;int *p=&a; return p;}