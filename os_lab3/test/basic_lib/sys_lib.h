//#ifndef _SYS_LIB_H_
//#define _SYS_LIB_H_
//#include "../include/defines.h"
//#define FLOPPY_SECTOR_PER_TRACK 18
//#define FLOPPY_TRACK_PER_HEAD 80
//#define FLOPPY_HEAD_PER_DISK 2
//#ifdef __cplusplus
//extern "C"{
//#endif
//    void sys_execve_bin();
//    void sys_dbg_bochs_putc(char c);
////    void sys_bios_print_string(const char* str, unsigned int len, int color, int pos);
//    void sys_bios_print_int(int num, int color, int pos);
////    void sys_bios_clear_screen();
//    void sys_clear_screen();
////    void sys_bios_putchar(char c, int color, int x, int y);
//    uint8_t sys_inb(uint16_t port);
//    void sys_outb(uint16_t port, uint8_t data);
//    void sys_outsl(uint32_t port, const void *addr, uint32_t cnt);
//    void sys_insl(uint32_t port, void *addr, uint32_t cnt);
//    void sys_outw(uint16_t port, uint16_t data);
//    char sys_get_scancode();
//    int  sys_getchar();
//    void vga_putchar(int c, int color, int x, int y);
//    void sys_print_string(const char* str, unsigned int len, int x, int y);
//    void sys_print_int(int num, int x, int y);
//    void sys_read_disk(uint32_t segment, uint32_t address, uint16_t logical_start_sector, uint8_t secotr_cnt);
//    void sys_read_hard_disk(uint32_t segment, uint32_t address, uint16_t logical_start_sector, uint8_t secotr_cnt);
////    void sys_bios_scroll_up(int color);
//    void sys_scroll_up();
//    void sys_sleep(int n);
//    void read_rtc();
//    extern char sys_internal_time_str[20];
//    #define bochs_break()       \
//        sys_outw(0x8A00,0x8A00);    \
//        sys_outw(0x8A00,0x08AE0);
//#ifdef __cplusplus
//}
//#endif
//
//#endif
