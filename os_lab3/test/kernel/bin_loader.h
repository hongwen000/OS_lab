//
// Created by 李新锐 on 30/03/2018.
//

#ifndef HHOS_BIN_LOADER_H
#define HHOS_BIN_LOADER_H

#include "../include/defines.h"
#include "../basic_lib/sys_lib.h"

class bin_loader
{
public:
    static void load_binary_from_floppy(int n)
    {
        sys_read_disk(user_prog_segment, user_prog_load_addr, n, 30);
        sys_execve_bin();
    }

private:
    static constexpr uint32_t user_prog_load_addr = 0xA100;
    static constexpr uint32_t user_prog_segment = 0x1000;
};

#endif //TEST_BIN_LOADER_H
