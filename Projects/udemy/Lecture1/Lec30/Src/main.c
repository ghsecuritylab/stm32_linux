#include "main.h"
#include<stdint.h>

extern void do_stack_operations(void);
int main(void)
{
    do_stack_operations();
    while(1){}
    return 0;
}


