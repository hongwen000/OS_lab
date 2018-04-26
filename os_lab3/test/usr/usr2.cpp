//
// Created by 李新锐 on 31/03/2018.
//

#include "../include/defines.h"
#include "../libc/stdio.h"
#include "../libc/ctype.h"
#include "../libc/time.h"
#include "../libc/unistd.h"


extern "C" void main()
{
//    char infix[SIZE];
//    printf("Basic calculate 1.1\n");
//    printf("Input q to exit\n");
//    printf("I\n");
//    putchar('I');
//    printf("%s\n",asctime(gmtime(time(nullptr))));
    while (true)
    {
        puts("In User Process 1\n");
    }
//    asm volatile("int $0x93");
    puts("User Process 1 Ends\n");
    while (1);
//    while (true)
//    {
//        gets(infix);
//        if (strcmp(infix, "q") == 0)
//            break;
//        calc c(infix);
//        int errorn = c.get_err();
//        if(errorn != 0)
//        {
//            printf("Wrong input at column %d\n", errorn);
//        }
//        else
//        {
//            printf("%d\n", c.get_result());
//        }
//    }
}

