#include <stdio.h>
#include <stdlib.h>
#include "struct.h"


int main(int argv, char* argvc[])
{
    struct_statement();

    // 结构成员访问
    struct_var();

    // 结构自引用 
    struct_own_var();

    // 结构、指针和成员
    struct_pointer_var();
}