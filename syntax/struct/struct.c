#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

// 结构声明
void struct_statement()
{
    // 直接创建结构变量
    struct {
        int a;
        char b;
        float c;
    } x;

    // 创建一个长度为20的数组，每个元素是一个结构体
    // 创建一个结构指针z
    struct {
        int a;
        char b;
        float c;
    } y[20], *z;

    // 使用标签
    struct SIMPLE {
        int a;
        char b;
        float c;
    };
    struct SIMPLE x1;

    // 使用别名
    typedef struct {
        int a;
        char b;
        float c;
    } simple;
    simple x2;
    simple y2[20], *z1;
}

// 结构成员的访问
void struct_var()
{
    struct SIMPLE {
        int a;
        char b;
        float c;
    };
    struct COMPLEX {
        float f;
        int a[20];
        long *lp;
        struct SIMPLE s;
        struct SIMPLE sa[10];
        struct SIMPLE *sp;
    };
    // 结构成员的直接访问
    struct COMPLEX commp = {3.14, {12,23,40}};
    // 通过.操作符直接访问
    float f01;
    f01 = commp.f;
    printf("The f is:%f\n", f01);
    // 结构的初始化
    struct COMPLEX commp_y[10] = {
                                    {3.14, {12,23,40}},
                                    {3.14, {12,23,40}}
                                 };
    float f02;
    f02 = commp_y[0].f;
    printf("The f is:%0.2f\n", f02);
    // 访问数组成员的元素
    int var01;
    var01 = commp_y[0].a[1];
    printf("This is var arr first:%d\n", var01);

    // 结构成员的间接访问 间接访问是通过结构指针访问
    struct COMPLEX *commp_y_p = &commp;
    float f03;
    f03 = (*commp_y_p).f;
    printf("This f is: %0.2f\n", f03);
    float f04;
    f04 = commp_y_p->f;  // ->操作符 对结构指针进行解引用
    printf("This f is: %0.2f\n", f04);
}

// 结构的自引用 
void struct_own_var()
{
    // 下面是一个非法的操作，成员b是一个完整的结构，
    // 其内部又有包含一个完成结构的b,形成了一个类型死循环的关系 
    // struct SELF_PEF1 {
    //     int a;
    //     struct SELF_REF1 b;
    //     int c;
    // };

    // 正确的方式应该是将成员定义成指针
    struct SELF_PEF1 {
        int a;
        struct SELF_REF1 *b;
        int c;
    };

    // 使用别名的陷阱
    // 注意这种声明是有问题的，*b并不知道SELF_REF3是谁
    // 通常会抛出一个编译错误：error: unknown type name 'SELF_REF3'
    // typedef struct {
    //     int a;
    //     SELF_REF3 *b;
    //     int c;
    // } SELF_REF3;
    // 来看一下正确的定义方式
    // 使用结构标签 来声明自引用，由于SELF_REF3_TAG在结构成员了之前。所以，
    // 在声明成员b的时候，它是知道要自己是谁的
    typedef struct SELF_REF3_TAG{
        int a;
        struct SELF_REF3_TAG *b;
        int c;
    } SELF_REF3;
}

// 不完整的声明
void struct_part_statement()
{
    // 如果两个结构相互引用，那我们应该先声明哪一个结构呢？
    // 这个时候我可以使用不完整声明
    struct a; // 这个时候a就是不完整声明 
    struct b {
        struct a *a1;
    };
    struct a {
        struct b *b1;
    };
}

// 结构成员、指针和成员
void struct_pointer_var()
{
    typedef struct {
        int a;
        short b[2];
    } Ex2;
    typedef struct EX {
        int a;
        char b[3];
        Ex2 c;
        struct EX *d;
    } Ex;
}