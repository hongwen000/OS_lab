#ifndef _HHOS_H_
#define _HHOS_H_
#include "../../kernel_lib/pm.h"

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

static inline void system_call_sleep()
{
    asm volatile(
    "movb $2, %%ah\n\t"
    "int $0x98\n\t"
    :
    :
    :"%eax", "%ebx"
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

static inline int fork()
{
    uint32_t ret;
    asm volatile(
    "movb $4, %%ah\n\t"
    "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    :"=r"(ret)
    :
    :"%eax", "%ebx", "%ecx"
    );
    return ret;
}
static inline int wait()
{
    uint32_t ret;
    asm volatile(
    "movb $5, %%ah\n\t"
    "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    :"=r"(ret)
    :
    :"%eax", "%ebx", "%ecx"
    );
    return ret;
}
static inline void exit()
{
    uint32_t ret;
    asm volatile(
    "movb $6, %%ah\n\t"
    "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    :"=r"(ret)
    :
    :"%eax", "%ebx", "%ecx"
    );
}
static inline int clone()
{
    uint32_t ret;
    asm volatile(
    "movb $7, %%ah\n\t"
    "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    :"=r"(ret)
    :
    :"%eax", "%ebx", "%ecx"
    );
    return ret;
}
static inline uint32_t exec(uint32_t n)
{
    uint32_t ret;
    asm volatile(
    "movb $8, %%ah\n\t"
    "push %1\n\t"
    "push $0\n\t"
    "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    "add $8, %%esp"
    :"=r"(ret)
    :"r"(n)
    :"%eax", "%ebx", "%ecx"
    );
    return ret;
}
static inline int p(int n)
{
    int ret;
    asm volatile(
    "movb $9, %%ah\n\t"
    "push %1\n\t"
    "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    "add $4, %%esp"
    :"=r"(ret)
    :"r"(n)
    :"%eax", "%ebx", "%ecx"
    );
    return ret;
}
static inline int v(int n)
{
    int ret;
    asm volatile(
    "movb $10, %%ah\n\t"
    "push %1\n\t"
    "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    "add $4, %%esp"
    :"=r"(ret)
    :"r"(n)
    :"%eax", "%ebx", "%ecx"
    );
    return ret;
}
static inline int getsem(int n)
{
    int ret;
    asm volatile(
    "movb $11, %%ah\n\t"
    "push %1\n\t"
    "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    "add $4, %%esp"
    :"=r"(ret)
    :"r"(n)
    :"%eax", "%ebx", "%ecx"
    );
    return ret;
}
static inline int freesem(int n)
{
    int ret;
    asm volatile(
    "movb $12, %%ah\n\t"
    "push %1\n\t"
    "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    "add $4, %%esp"
    :"=r"(ret)
    :"r"(n)
    :"%eax", "%ebx", "%ecx"
    );
    return ret;
}

static inline
void * sbrk(int incr)
{
    void* ret;
    asm volatile(
    "movb $13, %%ah\n\t"
    "push %1\n\t"
    "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    "add $4, %%esp"
    :"=r"(ret)
    :"r"(incr)
    :"%eax", "%ebx", "%ecx"
    );
    return ret;
}


static inline
int munmap(void *addr, size_t length)
{
    int ret;
    asm volatile(
    "movb $14, %%ah\n\t"
    "push %2\n\t"
    "push %1\n\t"
    "int $0x98\n\t"
    "movl %%eax, %0\n\t"
    "add $8, %%esp"
    :"=r"(ret)
    :"r"(addr), "r"(length)
    :"%eax", "%ebx", "%ecx"
    );
    return ret;
}

#endif

