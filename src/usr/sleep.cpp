#include "../libc/unistd.h"
#include "../libc/stdio.h"

extern "C" void main()
{
    for(unsigned int i = 1; i < 100000000; i *= 10)
    {
        printf("Testing sleep function\n");
        sleep(i);
        printf("Sleepd for %d \n", i);
    }
}
