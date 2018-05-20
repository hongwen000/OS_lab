#ifndef _SYS_LIB_H_
#define _SYS_LIB_H_
#include "../include/defines.h"
#include "./pm.h"
#include "./isr.h"
//#include "../proc/proc.h"

#define FLOPPY_SECTOR_PER_TRACK 18
#define FLOPPY_TRACK_PER_HEAD 80
#define FLOPPY_HEAD_PER_DISK 2
#ifdef __cplusplus
extern "C"{
#endif
    void blue_screen(int_frame *r);
    void sys_current_tty_putchar(int ch);
    int  sys_current_tty_get_color();
    void sys_current_tty_set_color(int color);
    void sys_new_proc(uint32_t SEL_CODE, uint32_t SEL_DATA);
    void sys_proc_save();
    void sys_dbg_bochs_putc(char c);

    void sys_execve_bin(uint32_t SEL_CODE, uint32_t SEL_DATA);

    /* vga */
    void sys_clear_screen();
    void sys_putchar(int c, int color, int x, int y);
    void sys_scroll_up();
    void sys_move_cursor(int x, int y);

    uint8_t sys_inb(uint16_t port);
    void sys_outb(uint16_t port, uint8_t data);
    void sys_outsl(uint32_t port, const void *addr, uint32_t cnt);
    void sys_insl(uint32_t port, void *addr, uint32_t cnt);
    void sys_outw(uint16_t port, uint16_t data);
    char sys_get_scancode();
    int  sys_getchar();
    void sys_print_string(const char* str, unsigned int len, int x, int y);
    void sys_print_int(int num, int x, int y);
    void sys_read_disk(uint32_t segment, uint32_t address, uint16_t logical_start_sector, uint8_t secotr_cnt);
    void sys_read_hard_disk(uint32_t segment, uint32_t address, uint16_t logical_start_sector, uint8_t secotr_cnt);
    void sys_sleep(int n);
    void read_rtc();
    extern char sys_internal_time_str[20];
    #define bochs_break()       \
        sys_outw(0x8A00,0x8A00);    \
        sys_outw(0x8A00,0x08AE0);
#ifdef __cplusplus
}
#endif

#endif
