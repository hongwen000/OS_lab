//
// Created by 李新锐 on 25/04/2018.
//

#ifndef HHOS_PROC_H
#define HHOS_PROC_H

#include "../include/defines.h"
#include "../kernel_lib/pm.h"
#include "../kernel_lib/sys_utility.h"

struct PCB{
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
    uint32_t user_esp;
    uint32_t ss;    // 16 bits
};

extern "C" void schedule(PCB*);


#endif //HHOS_PROC_H
