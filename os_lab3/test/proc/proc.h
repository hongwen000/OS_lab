//
// Created by 李新锐 on 25/04/2018.
//

#ifndef HHOS_PROC_H
#define HHOS_PROC_H

#include "../include/defines.h"
#include "../kernel_lib/pm.h"
#include "../kernel_lib/sys_utility.h"
#define MAX_PROC 4

#define PROC_RUNNING 1
#define PROC_WAITING 0

struct CPU_INFO{
    /* segment registers */
    uint32_t gs;    // 16 bits
    uint32_t fs;    // 16 bits
    uint32_t es;    // 16 bits
    uint32_t ds;          // 16 bits

    /* registers save by pusha */
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;


    /* save by `int` instruction */
    uint32_t eip;
    uint32_t cs;    // 16 bits
    uint32_t eflags;
}__attribute__((packed));

struct PCB
{
    uint32_t id;
    PCB* next;
    uint32_t status = PROC_WAITING;
    /* segment registers */
    uint32_t gs;    // 16 bits
    uint32_t fs;    // 16 bits
    uint32_t es;    // 16 bits
    uint32_t ds;          // 16 bits

    /* registers save by pusha */
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;


    /* save by `int` instruction */
    uint32_t eip;
    uint32_t cs;    // 16 bits
    uint32_t eflags;
};

extern "C" void round_robin();
extern "C" void save(CPU_INFO *cpu_info);
void proc_init();
void get_pcb();


#endif //HHOS_PROC_H
