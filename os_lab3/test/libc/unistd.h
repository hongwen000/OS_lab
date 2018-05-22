#include "sys/hhos.h"
static inline unsigned int sleep(unsigned int seconds)
{
    while (seconds--)
        system_call_sleep();
    return 0;
}

