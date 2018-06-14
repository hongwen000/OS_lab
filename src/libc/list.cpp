#include "list.h"
#include "stdio.h"

list::list() : head(nullptr), _size(0) { }

using list_iterator = node*;
using const_list_iterator = const node*;

list::list(const list& rhs) 
{
    _size = rhs._size;
    const_list_iterator rhs_iter = rhs.head;
    if(rhs_iter != nullptr)
        head = new node(rhs_iter->data);
    rhs_iter = rhs_iter->next;
    list_iterator this_iter = head;
    while (rhs_iter != nullptr) {
        this_iter->next = new node(rhs_iter->data);
        rhs_iter = rhs_iter->next;
        this_iter = this_iter->next;
    }
}

bool list::empty() const {
    return !_size;
}

void list::insert(int position, const int& data)
{
    list_iterator iter = this->head;
    if(position > _size) 
        return;
    else {
        if(head == nullptr) 
            head = new node(data);
        else if(position == 0) {
            node* tmp = new node(data);
            tmp->next = head;
            head = tmp;
        }
        else {
            for(int i = 0; i < position - 1; ++i)
               iter = iter->next;
            node* tmp = new node(data);
            if (iter != nullptr) {
                tmp->next = iter->next;
                iter->next = tmp;
            }
        }
        ++_size;
    }
} 

list::~list() {
    list_iterator iter = this->head;
    while(iter != nullptr) {
        list_iterator tmp = iter->next;
        delete iter;
        iter = tmp;
    }
}

void list::erase(int position) {
    list_iterator iter = this->head;
    if(position >= _size)
        return;
    else {
        if(position == 0) {
            delete head;
            head = iter->next;
        }
        else {
            for(int i = 0; i < position - 1; ++i)
               iter = iter->next;
            list_iterator tmp = iter->next;
            iter->next = tmp->next;
            delete tmp;
        }
        --_size;
    }
}

list& list::operator=(const list& rhs) {
    if (this == &rhs) return *this;
    list_iterator iter = this->head;
    while(iter != nullptr) {
        list_iterator tmp = iter->next;
        delete iter;
        iter = tmp;
    }
    _size = rhs._size;
    const_list_iterator rhs_iter = rhs.head;
    if(rhs_iter != nullptr)
        head = new node(rhs_iter->data);
    rhs_iter = rhs_iter->next;
    list_iterator this_iter = head;
    while (rhs_iter != nullptr) {
        this_iter->next = new node(rhs_iter->data);
        rhs_iter = rhs_iter->next;
        this_iter = this_iter->next;
    }
    return *this;
}


void list::print(void) const {
    const_list_iterator citer = head;
    while(citer != nullptr) {
        printf("%d", citer->data);
        printf("->");
        citer = citer->next;
    }
    printf("nullptr");
}

int list::size() const {
    return _size;
}
