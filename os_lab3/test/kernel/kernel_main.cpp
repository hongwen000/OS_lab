#include "./tty.h"
#include "./bin_loader.h"
#include "../basic_lib/sys_lib.h"
#include "../libc/string.h"
#include "../libc/stdio.h"
#include "sh.h"

static tty* current_tty = nullptr;
tty* sys_get_current_tty(){return current_tty;}
const char* str = "I am back";
extern "C" void kernel_main()
{
    tty tty1;
    current_tty = &tty1;
//    while (true)
//    {
//        int ch = getchar();
//        if (ch == 13)
//            putchar('\n');
//        else
//            putchar(ch);
//    }
    printf("%s\n", "tty init OK");
    sh sh1;
    printf("%s\n", "sh init OK");
    sh1.run();
    while(1);
}
