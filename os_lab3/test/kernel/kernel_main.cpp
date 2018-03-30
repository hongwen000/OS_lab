#include "./tty.h"
#include "../basic_lib/sys_lib.h"
#include "../libc/string.h"
#include "../libc/stdio.h"

static tty* current_tty = nullptr;
tty* sys_get_current_tty(){return current_tty;}

extern "C" void kernel_main()
{
    sys_bios_clear_screen();
    tty tty1;
    tty1.tty_init();
    current_tty = &tty1;
    while(1);
}
