#include "main.h"
#include <stdio.h>
/*
* SVC sample for GCC-Toolchain on Cortex-M3/M4 or M4F
*/

/*
* Inline assembler helper directive: call SVC with the given immediate
*/
#define svc(code) asm volatile ("svc %[immediate]"::[immediate] "I" (code) )
#define SVC_ADD_DATA        0x36
#define SVC_MULTIPLY_DATA   0x37
/*
* Handler function definition, same parameters as the SVC function below
*/

void sv_call_write_data_handler(int *svc_args, int* result);

/*
* Use a normal C function, the compiler will make sure that this is going
* to be called using the standard C ABI which ends in a correct stack
* frame for our SVC call
*/

__attribute__ ((noinline)) void sv_call_add_data(int a, int b, int* c)
{
    svc(SVC_ADD_DATA);
}

__attribute__ ((noinline)) void sv_call_multiply_data(int a, int b, int* c)
{
    svc(SVC_MULTIPLY_DATA);
}


/*
* SVC Handler entry, put a pointer to this function into the vector table
*/

int __attribute__ (( naked )) SVC_Handler(void)
{
    asm volatile(
        "tst lr, #4\t\n" // Check EXC_RETURN[2] 
        "ite eq\t\n"
        "mrseq r0, msp\t\n"
        "mrsne r0, psp\t\n"
        "b %[sv_call_write_data_handler]\t\n"
        :
        : [sv_call_write_data_handler] "i" (sv_call_write_data_handler) // input 
        : "r0" // clobber
    );
}

int main(void){
    int result = 2;
    sv_call_add_data(0x17, 0x27, &result);
    sv_call_multiply_data(0x17, 0x27, &result);

    return 0;
}

void sv_call_write_data_handler(int *svc_args, int* result){
    uint32_t svc_number;
    /* svc_args[6] will give address of next instruction after "svc 0x36" instruction 
       (svc_args[6]- 2) will give the address of "svc 0x36" instruction 
       so *( ((char *)(svc_args[6])) -2 ); will give the value of svc service number 
    */
    svc_number = *( ((char *)(svc_args[6])) -2 );

    switch(svc_number){
        case SVC_ADD_DATA :
        svc_args[0] = svc_args[0]+svc_args[1];
        break;
        case SVC_MULTIPLY_DATA :
        svc_args[0] = svc_args[0] * svc_args[1];
        break;
        default:
        break;
    }
    *result = svc_args[0];
}
