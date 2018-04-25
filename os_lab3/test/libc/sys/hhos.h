#ifndef _HHOS_H_
#define _HHOS_H_
#include "../../kernel/tty.h"
#include "../../kernel_lib/pm.h"
#include "../../kernel_lib/sys_utility.h"
tty* sys_get_current_tty();

#define __ISR_BEGIN_SAFE_C__        \
        asm volatile(               \
        "pusha\n\t"                 \
        "push ds\n\t"               \
        "push es\n\t"               \
        "push fs\n\t"               \
        "push gs\n\t"               \
        "push ax\n\t"               \
        "mov ax, SEL_KERN_DATA\n\t" \
        "mov ds, ax\n\t"            \
        "mov es, ax\n\t"            \
        "mov fs, ax\n\t"            \
        "mov ss, ax\n\t"            \
        "mov ax, SEL_KERN_VIDEO\n\t"\
        "mov gs, ax\n\t"            \
        "pop ax    \n\t"            \
        )

#define __ISR_LEAVE_SAFE_C__       \
        asm volatile(              \
        "pop gs\n\t"               \
        "pop fs\n\t"               \
        "pop es\n\t"               \
        "pop ds\n\t"               \
        "popa  \n\t"               \
        )

static inline void system_call_sleep(unsigned int n)
{
    asm volatile(
    "movl %0, %%ecx\n\t"
    "movb $2, %%ah\n\t"
            "int $0x98\n\t"
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
            "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    :"=r"(ret)
    :
    :"%eax"
    );
    int mask = 0xFFFF;
    ret = ret & mask;
    return ret;
}
static void system_call_putchar(int ch)
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
    :"%eax", "ebx"
    );
    //TODO
}
static inline uint32_t system_call_get_timestamp()
{
    uint32_t ret;
    asm volatile(
            "movb $3, %%ah\n\t"
                    "int $0x98\n\t"
            "movl %%eax, %0"
            :"=r"(ret)
            :
            :"%eax", "%ebx", "%ecx"
            );
    return ret;
}
#endif
