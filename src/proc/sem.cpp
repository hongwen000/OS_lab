//
// Created by 李新锐 on 11/06/2018.
//
#include "../include/defines.h"
#include "proc.h"
#include "../kernel_lib/debug_printf.h"

#define NSEM 100
struct Sem {
    int v;
    bool used;
    PCB* head;
};
Sem sem[NSEM];

int sys_do_p(int sem_id) {
    __sync_fetch_and_sub(&sem[sem_id].v, 1);
//    --sem[sem_id].v;
    debug_printf("sys_do_p: sem[%d] = %d\n", sem_id, sem[sem_id].v);
    if(sem[sem_id].v < 0)
    {
        current_proc->next = sem[sem_id].head;
        sem[sem_id].head = current_proc;
        sys_do_sleep(&sem[sem_id]);
    }
    return 0;
}

int sys_do_v(int sem_id) {
    __sync_fetch_and_add(&sem[sem_id].v, 1);
//    ++sem[sem_id].v;
    debug_printf("sys_do_v: sem[%d] = %d\n", sem_id, sem[sem_id].v);
    if(sem[sem_id].v <= 0)
    {
        wakeup_one(&sem[sem_id], sem[sem_id].head);
        sem[sem_id].head = current_proc->next;
        //假设一个信号量仅被一个进程进行v操作，那么这样能够保证强信号量
//        go_back_scheduler();
    }
    return 0;
}

int sys_do_getsem(int v) {
    for(int i = 0; i < NSEM; ++i)
    {
        if(!sem[i].used)
        {
            sem[i].v = v;
            sem[i].used = true;
            debug_printf("sys_do_getsem: give you sem[%d]\n", i);
            return i;
        }
    }
    return -1;
}

int sys_do_freesem(int sem_id) {
    sem[sem_id].used = false;
    sem[sem_id].head = nullptr;
    return 0;
}
