#include "base_lib.h"

template<int N>
constexpr int fibonacci()
{
    if constexpr (N>=2)
        return fibonacci<N-1>() + fibonacci<N-2>();
    else
        return N;
}

extern "C" void kernel_start()
{
    clear_screen();
    char arr[] = "Hello, world";
    print_string(arr, 12, MAKE_COLOR(BLACK, WHITE), MAKE_POS(0, 0));
    int year = 1998;
    print_int(year, MAKE_COLOR(BLACK, WHITE), MAKE_POS(1,0));
    int fib = fibonacci<20>();
    print_int(fib, MAKE_COLOR(BLACK, WHITE), MAKE_POS(2,0));
    while(1);
}
