#ifndef _SYS_LIB_H_
#define _SYS_LIB_H_
#include "../include/defines.h"
#define FLOPPY_SECTOR_PER_TRACK 18
#define FLOPPY_TRACK_PER_HEAD 80
#define FLOPPY_HEAD_PER_DISK 2
#ifdef __cplusplus
extern "C"{
#endif
    void sys_execve_bin();
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
    void sys_read_disk(uint32_t segment, uint32_t address, uint16_t logical_start_sector, uint8_t secotr_cnt);

#ifdef __cplusplus
}
#endif

#endif