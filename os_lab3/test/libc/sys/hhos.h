#ifndef _HHOS_H_
#define _HHOS_H_
#include "../../kernel/tty.h"
#include "../../basic_lib/sys_lib.h"
tty* sys_get_current_tty();

#define INVOKE_INT_SAFE(INT_N) \
            "movw %%ss, %%bx\n\t" \
            "movw %%bx, %%fs\n\t" \
            "movw $0, %%bx\n\t" \
            "movw %%bx, %%ss\n\t" \
            "movw %%bx, %%ds\n\t" \
            "movw %%bx, %%es\n\t" \
    "int $0x"#INT_N"\n\t" \
            "movw %%fs, %%bx\n\t" \
            "movw %%bx, %%ss\n\t" \
            "movw %%bx, %%ds\n\t" \
            "movw %%bx, %%es\n\t" \


static inline void system_call_sleep(unsigned int n)
{
    asm volatile(
    "movl %0, %%ecx\n\t"
    "movb $2, %%ah\n\t"
    INVOKE_INT_SAFE(98)
    :
    :"r"(n)
    :"%eax", "%ebx", "%ecx"
    );

}

static inline int system_call_getchar()
{
    int ret = 0;
    asm volatile (
    "movb $0, %%ah\n\t"
    INVOKE_INT_SAFE(98)
    "movl %%eax, %0\n\t"
    :"=r"(ret)
    :
    :"%eax"
    );
    int mask = 0xFFFF;
    ret = ret & mask;
    return ret;
}
static inline void system_call_putchar(int ch)
{
    int mask = 0x0FF;
    ch = ch & mask;
    char c = ch;
    asm volatile(
    "movb %0, %%al\n\t"
    "movb $1, %%ah\n\t"
    INVOKE_INT_SAFE(98)

    :
    :"r"(c)
    :"%eax", "ebx"
    );
    //TODO
}
#endif
