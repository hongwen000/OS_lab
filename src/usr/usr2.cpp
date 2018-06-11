//
// Created by 李新锐 on 31/03/2018.
//

#include "../include/defines.h"
#include "../libc/sys/hhos.h"
#include "../libc/stdio.h"
#include "../libc/ctype.h"
#include "../kernel_lib/sys_utility.h"

int resource = 0;
int s;

void main() {
    s = getsem(0);
    if (clone())
    {
        while (1) {
            p(s);
            printf("Consumer: %d\n", resource);
            --resource;
        }
    }
    else
    {
        while (1) {
            ++resource;
            v(s);
            printf("Producer: %d\n", resource);
        }
    }
}

