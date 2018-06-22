#include "../include/defines.h"
#include "../libc/list.h"
#include "../libc/stdio.h"

int pos[] = {1,2,0,1,2,3,4,5,6,7};
int data[] = {1,1,1,2,3,4,5,6,7,8};
int del[] = {5,8,3,2,1};
extern "C" void main() {
  list li;
 
  int n = 10;

  for (int i = 0; i < n; i++) {
    li.insert(pos[i], data[i]);
  }

  li.print();
  printf("\n");

  list li2(li);
  list li3;
 
  li = li3 = li2 = li;

  li.print();
  printf("size: %d\n", li.size());
  li2.print();
  printf("size: %d\n", li2.size());
  li3.print();
  printf("size: %d\n", li3.size());

  int m = 5;

  for (int i = 0; i < m; i++) {
    li.erase(del[i]);
  }

  li.print();
  printf("\n");
}
 
