//
// Created by 李新锐 on 21/05/2018.
//

#ifndef TEST_SYS_PROC_H
#define TEST_SYS_PROC_H

#include "../include/defines.h"
#include "proc.h"

int sys_fork();
int sys_fork_thread();
int sys_exit();
int sys_wait();
int sys_sleep(int n);

#endif //TEST_SYS_PROC_H
