#ifndef _HHOS_H_
#define _HHOS_H_
#include "../../kernel/tty.h"
#include "../../basic_lib/sys_lib.h"
tty* sys_get_current_tty();

static inline int system_call_getchar()
{
    int ret = 0;
    asm volatile (
    "movb $0, %%ah\n\t"
    "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    :"=r"(ret)
    :
    :"%eax"
    );
    int mask = 0x00FF;
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
    "int $0x98\n\t"
    :
    :"r"(c)
    :"%eax"
    );
}
#endif