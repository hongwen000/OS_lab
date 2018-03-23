#include "base_lib.h"

template<int N>
constexpr int fibonacci()
{
    if constexpr (N>=2)
        return fibonacci<N-1>() + fibonacci<N-2>();
    else
        return N;
}

const char* arr = "Hello, world";

extern "C" void kernel_main()
{
    sys_bios_clear_screen();
    int year = 1998;
    sys_bios_print_int(year, MAKE_COLOR(VGA_BLACK, VGA_WHITE), MAKE_POS(1,0));
    int fib = fibonacci<20>();
    sys_bios_print_int(fib, MAKE_COLOR(VGA_BLACK, VGA_WHITE), MAKE_POS(2,0));
    char arr[] = "Hello, world";
    sys_bios_print_string(arr, 12, MAKE_COLOR(VGA_BLACK, VGA_YELLOW), MAKE_POS(0, 0));
    int i = 1;
    while(i++) {
        int num = sys_get_scancode();
        sys_bios_print_int(num, MAKE_COLOR(VGA_BLACK, VGA_WHITE), MAKE_POS((5+i),5));
    }
}
