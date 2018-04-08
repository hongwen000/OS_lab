#include "sys/hhos.h"
static inline unsigned int sleep(unsigned int seconds)
{
    system_call_sleep(seconds);
    return 0;
}

