#ifndef _SYS_LIB_H_
#define _SYS_LIB_H_
#include "../include/defines.h"

#ifdef __cplusplus
extern "C"{
#endif 
    void sys_dbg_bochs_putc(char c);
    void sys_bios_print_string(const char* str, unsigned int len, int color, int pos);
    void sys_bios_print_int(int num, int color, int pos);
    void sys_bios_clear_screen();
    void sys_bios_putchar(char c, int color, int x, int y);
    int  sys_bios_getchar();
    uint8_t sys_inb(uint16_t port);
    void sys_outb(uint16_t port, uint8_t data);
    char sys_get_scancode();
    int  sys_getchar();
    void sys_putchar(int c, int color, int x, int y);
    void sys_print_string(const char* str, unsigned int len, int x, int y);
    void sys_print_int(int num, int x, int y);

#ifdef __cplusplus
}
#endif

#endif