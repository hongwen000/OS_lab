//
// Created by 李新锐 on 31/03/2018.
//

#include "../include/defines.h"
#include "../libc/sys/hhos.h"
#include "../libc/stdio.h"
#include "../libc/ctype.h"

char str[80] = "129djwqhdsajd128dw9i39ie93i8494urjoiew98kdkd";
int LetterNr = 0;
void CountLetter()
{
    for(int i = 0; i < 80; ++i)
    {
        if(isalpha(str[i]))
            ++LetterNr;
    }
}

extern "C" void main() {
    int pid;
    bochs_break();
    pid = clone();
    if (pid == -1)
        printf("error in fork !");
    if (pid) {
        wait();
        printf("LetterNr = %d\n", LetterNr);
    }
    else {
        bochs_break();
        CountLetter();
        exit();
    }
}

void sys_outw(uint16_t port, uint16_t data)
{
    asm volatile ("outw %0, %1"
    :
    :"a"(data), "Nd"(port));
}
//extern "C" void main()
//{
////    char infix[SIZE];
////    printf("Basic calculate 1.1\n");
////    printf("Input q to exit\n");
////    printf("I\n");
////    putchar('I');
////    printf("%s\n",asctime(gmtime(time(nullptr))));
//
////    while (true)
//        puts("In User Process 0\n");
////    asm volatile("int $0x93");
////    puts("User Process 0 Ends\n");
////    while (true)
////    {
////        gets(infix);
////        if (strcmp(infix, "q") == 0)
////            break;
////        calc c(infix);
////        int errorn = c.get_err();
////        if(errorn != 0)
////        {
////            printf("Wrong input at column %d\n", errorn);
////        }
////        else
////        {
////            printf("%d\n", c.get_result());
////        }
////    }
//}

