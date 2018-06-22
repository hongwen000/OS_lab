//
// Created by 李新锐 on 22/05/2018.
//

#ifndef TEST_EXEC_H
#define TEST_EXEC_H

#include "../include/defines.h"

#define MAX_ARGC 20

int sys_do_exec(uint32_t n);
int sys_do_exec(const char* path);
#endif //TEST_EXEC_H
