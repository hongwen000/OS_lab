//
// Created by 李新锐 on 31/03/2018.
//

#include "../include/defines.h"
#include "../libc/sys/hhos.h"
#include "../libc/stdio.h"
#include "../libc/ctype.h"
#include "../kernel_lib/sys_utility.h"
#include "../kernel_lib/debug_printf.h"

int resource = 0;
int s;
int n;

void main() {
    s = getsem(1);
    printf("usr2: got sem[%d]\n", s);
    n = getsem(0);
    printf("usr2: got sem[%d]\n", n);
    if (clone())
    {
        //Consumer
        while (1) {
//            printf("th1: got sem[%d]\n", s);
//            printf("th1: got sem[%d]\n", n);
            p(n);
            p(s);
            printf("Consumer: %d\n", resource);
            --resource;
            v(s);
        }
    }
    else
    {
        //Producer
        while (1) {
//            printf("th2: got sem[%d]\n", s);
//            printf("th2: got sem[%d]\n", n);
            p(s);
            ++resource;
            printf("Producer: %d\n", resource);
            v(s);
            v(n);
        }
    }
}

