#ifndef LIST
#define LIST

#include "new.h"
typedef struct node {
  int data;
  struct node* next;
  node(int data = 0, struct node* next = nullptr) : data(data), next(next) {}
} node;
 
class list {
 private:
  node* head;
  int _size;
 
 public:
  list();
  list(const list&);
  list& operator=(const list&);
  ~list();
 
  // Capacity
  bool empty(void) const;
  int size(void) const;
 
 public:
  // output
  // list: [1,2,3,4,5]
  // output: 1->2->3->4->5->nullptr

  void print(void) const;
  void insert(int position, const int& data);
  void erase(int position);
  void clear(void) {
    if (this->head != nullptr) {
      node* p = this->head;
      while (p != nullptr) {
        node* temp = p;
        p = p->next;
        delete temp;
      }
      this->head = nullptr;
    }
    this->_size = 0;
  }
};
 
#endif
