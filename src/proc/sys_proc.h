//
// Created by 李新锐 on 21/05/2018.
//

#ifndef TEST_SYS_PROC_H
#define TEST_SYS_PROC_H

#include "../include/defines.h"
#include "proc.h"
#include "exec.h"
#include "sem.h"

int sys_fork();
int sys_clone();
int sys_exit();
int sys_wait();
int sys_sleep(int n);
int sys_exec(uint32_t n);
int sys_p(int sem_id);
int sys_v(int sem_id);
int sys_getsem(int v);
int sys_freesem(int sem_id);


#endif //TEST_SYS_PROC_H
