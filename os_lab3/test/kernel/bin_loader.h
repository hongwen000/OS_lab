//
// Created by 李新锐 on 30/03/2018.
//

#ifndef HHOS_BIN_LOADER_H
#define HHOS_BIN_LOADER_H

#include "../include/defines.h"
#include "../kernel_lib/sys_utility.h"
#include "../kernel_lib/pm.h"
#include "../proc/proc.h"
#include "../libc/stdio.h"
#define offset(A, b) (&(((A *)0)->b))
static  int aaaa;
extern PCB* pcb_que;
class bin_loader
{
public:
    static void load_binary_from_disk(uint32_t SEL_DATA, int n)
    {
        sys_read_hard_disk(SEL_DATA, user_prog_load_addr, n, 40);
    }
    static void new_proc(uint32_t SEL_CODE, uint32_t SEL_DATA)
    {
        printf("pid addr: %x\n", offset(PCB, id));
        printf("pid addr: %x\n", offset(PCB, esp));
        get_pcb();
        asm volatile("int $0x92");
    }

private:
    static constexpr uint32_t user_prog_load_addr = USER_PROG_LOAD_ADDR;
//    static constexpr uint32_t user_prog_segment = USER_PROG_LOAD_SEG;
};

#endif //TEST_BIN_LOADER_H
