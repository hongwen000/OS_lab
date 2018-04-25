//
// Created by 李新锐 on 30/03/2018.
//

#ifndef HHOS_BIN_LOADER_H
#define HHOS_BIN_LOADER_H

#include "../include/defines.h"
#include "../kernel_lib/sys_utility.h"
#include "../kernel_lib/pm.h"

class bin_loader
{
public:
    static void load_binary_from_disk(uint32_t SEL_CODE, uint32_t SEL_DATA, int n)
    {
        sys_read_hard_disk(SEL_DATA, user_prog_load_addr, n, 40);
        sys_execve_bin(SEL_CODE, SEL_DATA);
    }

private:
    static constexpr uint32_t user_prog_load_addr = USER_PROG_LOAD_ADDR;
//    static constexpr uint32_t user_prog_segment = USER_PROG_LOAD_SEG;
};

#endif //TEST_BIN_LOADER_H
