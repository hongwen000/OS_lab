//
// Created by 李新锐 on 31/03/2018.
//

#include "../include/defines.h"
#include "../libc/sys/hhos.h"
#include "../libc/stdio.h"
#include "../libc/ctype.h"
#include "../kernel_lib/sys_utility.h"
#include "../libc/string.h"

int reader_count = 0;
int reader_sem;
int writer_sem;
char buf[21];
int ptr = 0;
void reader(int id)
{
    bool stop = false;
    while (!stop)
    {
        p(reader_sem);
        reader_count++;
        printf("Reader %d entered, %d readers in total\n", id, reader_count);
        if(reader_count == 1)
            p(writer_sem);
        v(reader_sem);
        printf("Reader %d start reading: %s\n", id, buf);
        if(strlen(buf) >= 10)
            stop = true;
        p(reader_sem);
        reader_count--;
        printf("Reader %d left, %d readers stays\n", id, reader_count);
        if(reader_count == 0)
            v(writer_sem);
        v(reader_sem);
    }
}
extern "C" void main()
{
    reader_sem = getsem(1);
    writer_sem = getsem(1);

    if(clone())
    {
        reader(1);
        wait();
    }
    else if(clone())
    {
        reader(2);
        wait();
    }
    else if(clone())
    {
        reader(3);
        wait();
    }
    else
    {
        while (ptr < 10)
        {
            p(writer_sem);
            buf[ptr] = 'A';
            int n = 100000;
            while (n--);
            printf("Writer: wrote 'A' to buffer\n");
            ++ptr;
            v(writer_sem);
        }
        printf("Writer finished his work\n");
        wait();
    }
}
//char str[80] = "129djwqhdsajd128dw9i39ie93i8494urjoiew98kdkd";
//int LetterNr = 0;
//void CountLetter()
//{
//    for(int i = 0; i < 80; ++i)
//    {
//        if(isalpha(str[i]))
//            ++LetterNr;
//    }
//}
//
//extern "C" void main() {
//    int pid;
//    pid = clone();
//    if (pid == -1)
//        printf("error in fork !");
//    if (pid) {
//        wait();
//        printf("LetterNr = %d\n", LetterNr);
//    }
//    else {
//        CountLetter();
//        exit();
//    }
//}
//
//void sys_outw(uint16_t port, uint16_t data)
//{
//    asm volatile ("outw %0, %1"
//    :
//    :"a"(data), "Nd"(port));
//}
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

