#include "sys_lib.h"
void sys_bios_print_int(int num, int color, int pos)
{
    int num_ = num;
    int len = 1;
    while(num_ /= 10) len++;
    char arr[len + 1];
    int power = 1;
    for(int i = 0; i < len; ++i, power *= 10)
        arr[len - i - 1] = '0' + ((num / power) % 10);
    arr[len] = '\0';
    sys_bios_print_string(arr, len, color, pos);
}

void sys_putchar(char c, int color, int x, int y)
{
     volatile uint16_t * pVRAM = (volatile uint16_t *) 0xB8000;
     pVRAM = pVRAM + (y * 80 + x) ;
     *pVRAM = c | (color << 8);
}

void sys_bios_putchar(char c, int color, int x, int y)
{
    sys_bios_print_string(&c, 1, color, MAKE_POS(x, y));
}

inline uint8_t sys_inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

// uint8_t sys_inb(uint16_t port)
// {
//     uint8_t ret;
//     asm volatile ( "inb al, dx"         //*****这而我当时错了，计组汇编诶
//                    : "=a"(ret)
//                    : "Nd"(port) );
//     return ret;
// }

char sys_get_scancode()
{
    while (!(sys_inb(0x64) & 1));
    return sys_inb(0x60);
}