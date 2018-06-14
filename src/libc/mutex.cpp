//
// Created by 李新锐 on 14/06/2018.
//
#include "mutex.h"
int mutex_get() {
    return getsem(1);
}

void mutex_lock(int lock) {
    p(lock);

}

void mutex_unlock(int lock) {
    v(lock);
}
