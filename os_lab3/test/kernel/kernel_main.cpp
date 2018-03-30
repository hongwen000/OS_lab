#include "./tty.h"
#include "./bin_loader.h"
#include "../basic_lib/sys_lib.h"
#include "../libc/string.h"
#include "../libc/stdio.h"

static tty* current_tty = nullptr;
tty* sys_get_current_tty(){return current_tty;}
const char* str = "I am back";
extern "C" void kernel_main()
{
    sys_bios_clear_screen();
    tty tty1;
    tty1.tty_init();
    current_tty = &tty1;
    printf("%s\n", str);
    bin_loader::load_binary_from_floppy();
    printf("%s", str);
    while(1);
}
