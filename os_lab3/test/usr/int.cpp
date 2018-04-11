#include "../include/defines.h"
#include "../libc/stdio.h"
extern "C" void main()
{
    printf("In int test program\n");
    asm volatile(
            "movw %%ss, %%bx\n\t"
            "movw %%bx, %%fs\n\t"
            "movw $0, %%bx\n\t"
            "movw %%bx, %%ss\n\t"
            "movw %%bx, %%ds\n\t"
            "movw %%bx, %%es\n\t"
    "int $0x33\n\t"
            "movw %%fs, %%bx\n\t"
            "movw %%bx, %%ss\n\t"
            "movw %%bx, %%ds\n\t"
            "movw %%bx, %%es\n\t"
            :
            :
            :"%ebx"
            );
    asm volatile(
        INVOKE_INT_SAFE(34)
        :
        :
        :"%ebx"
            );
    while(getchar() != 27);
}
