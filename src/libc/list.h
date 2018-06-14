#ifndef LIST
#define LIST

#include "new.h"
struct node {
  int data;
  struct node* next;
  node(int data = 0, struct node* next = nullptr) :
          data(data), next(next) {}
};
 
class list {
 private:
  node* head;
  int _size;
 
 public:
  list();
  list(const list&);
  list& operator=(const list&);
  ~list();
  bool empty(void) const;
  int size(void) const;
 public:
  void print(void) const;
  void insert(int position, const int& data);
  void erase(int position);
  void clear(void);
};
 
#endif
