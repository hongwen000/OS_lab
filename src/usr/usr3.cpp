//
// Created by 李新锐 on 31/03/2018.
//

#include "../include/defines.h"
#include "../libc/sys/hhos.h"
#include "../libc/stdio.h"
#include "../libc/ctype.h"
#include "../kernel_lib/sys_utility.h"
#include "../kernel_lib/debug_printf.h"

int resource = 1000;
int s;
int n;

void main() {
    s = getsem(1);
    printf("usr3: got sem[%d]\n", s);
    n = getsem(0);
    printf("usr3: got sem[%d]\n", n);
    if (clone())
    {
        //Consumer
        for(int i = 0; i < 100; ++i)
        {
            p(s);
            resource += 10;
            printf("Father: deposited 10$, now %d$ in bank\n", resource);
            v(s);
        }
    }
    else
    {
        //Producer
        for(int i = 0; i < 50; ++i) {
            p(s);
            resource -= 20;
            printf("Child: withdrew 20$, now %d$ in bank\n", resource);
            v(s);
        }
    }
}

