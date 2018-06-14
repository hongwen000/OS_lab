//
// Created by 李新锐 on 14/06/2018.
//

#ifndef SRC_MUTEX_H
#define SRC_MUTEX_H

#include "sys/hhos.h"
using mutex_t = int;
int mutex_get();
void mutex_lock(int lock);
void mutex_unlock(int lock);
#endif //SRC_MUTEX_H
