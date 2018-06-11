//
// Created by 李新锐 on 25/04/2018.
//

#ifndef HHOS_PROC_H
#define HHOS_PROC_H

#include "../include/defines.h"
#include "../kernel_lib/pm.h"
#include "../kernel_lib/sys_utility.h"
#include "../kernel_lib/isr.h"

#define MAX_PROC 128

#define PROC_STAT_NOT_READY 0
#define PROC_STAT_READY 1
#define PROC_STAT_RUNNING 2
#define PROC_STAT_WAITING 3

//struct CPU_INFO_t{
//    /* segment registers */
//    uint32_t gs;    // 16 bits
//    uint32_t fs;    // 16 bits
//    uint32_t es;    // 16 bits
//    uint32_t ds;          // 16 bits
//
//    /* registers save by pusha */
//    uint32_t edi;
//    uint32_t esi;
//    uint32_t ebp;
//    uint32_t esp;
//    uint32_t ebx;
//    uint32_t edx;
//    uint32_t ecx;
//    uint32_t eax;
//
//    uint32_t int_no;
//
//    /* save by `int` instruction */
//    uint32_t err_code;
//    uint32_t eip;
//    uint32_t cs;    // 16 bits
//    uint32_t eflags;
//    uint32_t user_esp;
//    uint32_t ss;    // 16 bits
//}__attribute__((packed));

struct context_t {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip;
};
enum procstate { P_UNUSED, P_USED, P_SLEEPING, P_RUNNABLE, P_RUNNING, P_ZOMBIE };
struct PCB {
    uint32_t text_size;                     // Size of process memory (bytes)
    pde_t* pgdir;                // Page table
    char *kern_stack;                // Bottom of kernel stack for this process
    enum procstate state;        // Process state
    int pid;                     // Process ID
    PCB *parent;         // Parent process
    int_frame *tf;        // Trap frame for current syscall
    context_t *context;     // swtch() here to run process
    void *sleep_event;                  // If non-zero, sleeping on sleep_event
    int killed;                  // If non-zero, have been killed
//    struct file *ofile[NOFILE];  // Open files
    struct inode *cwd;           // Current directory
    char name[16];               // Process name (debugging)
    PCB* next;
};

extern PCB *current_proc;
extern context_t *cpu_context;
void proc_init();
void scheduler();
int sys_do_fork();
int sys_do_clone();
void sys_do_exit();
int sys_do_wait();
void wakeup(void *sleep_event);
void wakeup_one(void *sleep_event, PCB* pp);
void sys_do_sleep(void *sleep_event);
void sched();

#endif //HHOS_PROC_H
