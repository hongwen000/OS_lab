#include "../include/defines.h"
#include "../libc/stdio.h"
#include "../libc/time.h"
#include "../libc/unistd.h"
#include "../libc/assert.h"
#include "../libc/stdlib.h"
extern "C" void main()
{
    asm volatile(
        INVOKE_INT_SAFE(33):::"%ebx"
            );
    asm volatile(
        INVOKE_INT_SAFE(34):::"%ebx"
            );
    printf("In int test program\n");
    printf("Current unix timestamp is %u\n", time(nullptr));
    printf("Current datetime is %s\n", asctime(gmtime(time(nullptr))));
    printf("Testing sleep\n");
    sleep(40);
    printf("After 40 times of sleep\nNow it is %s\n",
        asctime(gmtime(time(nullptr))));
    hhos_assert(strlen("123") == 2);
    srand(time(nullptr));
    printf("Generated random number %d\n", rand());
    printf("Enter exit to exit\n");
    char buf[5];
    while(true)
    {
        scanf("%s", buf);
        buf[4] = 0;
        if(strcmp(buf, "exit") == 0)
            break;
    }
}
