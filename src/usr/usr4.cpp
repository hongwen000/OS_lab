#include "../include/defines.h"
#include "../libc/list.h"
#include "../libc/stdio.h"

int del[] = {5,8,3,2,1};
extern "C" void main() {
    list li;

    int n = 8;

    for (int i = 0; i < n; i++) {
        li.insert(0, n - i);
    }

    li.print();
    printf("\n");

    list li2(li);
    list li3;

    li = li3 = li2 = li;

    li2.insert(0, -2);
    li3.insert(0, -3);
    li.print();
    printf(" size: %d\n", li.size());
    li2.print();
    printf(" size: %d\n", li2.size());
    li3.print();
    printf(" size: %d\n", li3.size());

    int m = 5;

    for (int i = 0; i < m; i++) {
        li.erase(del[i]);
    }

    li.print();
    printf("\n");
}


