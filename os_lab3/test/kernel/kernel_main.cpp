#include "./tty.h"
#include "./bin_loader.h"
#include "../basic_lib/sys_lib.h"
#include "../libc/string.h"
#include "../libc/stdio.h"
#include "../libc/sys/hhos.h"
#include "sh.h"
static tty* current_tty = nullptr;
tty* sys_get_current_tty(){return current_tty;}
extern "C" void sys_current_tty_putchar(int ch)
{
    int mask = 0x00FF;
    ch = ch & mask;
    sys_get_current_tty()->putchar(ch);
}
const char* str = "Welcome to HHOS version 1.2.0\nYou can input help to see how to use it!";
static inline void print_ok(char * mod) {
    printf("%s init [", mod);
    current_tty->set_color(MAKE_COLOR(VGA_BLACK, VGA_GREEN));
    printf("OK");
    current_tty->set_color(MAKE_COLOR(VGA_BLACK, VGA_WHITE));
    printf("]\n");
}
extern "C" void kernel_main()
{
    tty tty1;
    current_tty = &tty1;
    print_ok("TTY");
    sh sh1;
    print_ok("Shell");
    printf("%s\n", str);
    sh1.run();
    while(1);
}
