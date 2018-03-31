//
// Created by 李新锐 on 31/03/2018.
//

#ifndef HHOS_SH_H
#define HHOS_SH_H

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "sys_lib.h"
#include "../basic_lib/sys_lib.h"
#include "bin_loader.h"
#include "../libc/sys/hhos.h"

class sh{
private:
    static constexpr int buf_size = 512;
    static constexpr int SUCCESS = 0;
    char buf[buf_size];
    char record_buf[512];
    char help[512];
    struct prog_entry {
        char name[32];
        int lba;
    };
    prog_entry progs[16];
    size_t prog_cnt = 0;
    size_t pos = 0;
    static constexpr char * prompt = "HHOS> ";
    int exec()
    {
        if (strcmp(buf, "ls") == 0|| strcmp(buf, "dir") == 0)
        {
            printf("You have %d user programs intalled\n\n", prog_cnt);
            for(size_t i = 0; i < prog_cnt; ++i)
            {
                printf("%s\n", progs[i].name);
            }
        } else if (strcmp(buf, "cls") == 0 || strcmp(buf, "clear") == 0)
        {
            sys_bios_clear_screen();
            sys_get_current_tty()->tty_init();
        } else if (strcmp(buf, "help") == 0)
        {
            printf("%s\n", help);

        }
        else {
            bool found = false;
            for(size_t i = 0; i < prog_cnt; ++i)
            {
                if (strcmp(buf, progs[i].name) == 0)
                {
                    found = true;
                    bin_loader::load_binary_from_floppy(progs[i].lba);
                    sys_bios_clear_screen();
                    sys_get_current_tty()->tty_init();
                    break;
                }
            }
            if (!found)
                printf("%s\n", "No such command or file");
        }
        return SUCCESS;
    }
    void read_prog_record()
    {
        sys_read_disk(0, (uint32_t)record_buf, 34, 1);
        char buf1[32];
        char buf2[32];
        char buf3[32];
        char * p = record_buf;
        sscanf(p, "%s", buf1);
        p += (strlen(buf1) + 1);
        prog_cnt = strtol(buf1, nullptr, 10);
        for(size_t i = 0; i < prog_cnt; ++i)
        {
            sscanf(p, "%s%s", buf2, buf3);
            strcpy(progs[i].name, buf2);
            progs[i].lba = strtol(buf3, nullptr, 10);
            p += (strlen(buf2) + strlen(buf3) + 2);
        }
    }
    void read_help_file()
    {
        sys_read_disk(0, (uint32_t)help, 32, 1);
    }
public:
    sh(){
        memset(buf, 0, buf_size);
        read_prog_record();
        read_help_file();
    }
    void run () {
        printf("%s", prompt);
        while(true)
        {
            char in = getchar();
            if (in == '\b')
            {
                if(pos != 0){
                    putchar('\b');
                    buf[pos--] = '\0';
                }
            }
            else if (in == 13)
            {
                putchar('\n');
                exec();
                memset(buf, 0, buf_size);
                pos = 0;
                printf("%s", prompt);
            }
            else {
                putchar(in);
                buf[pos++] = in;
            }
        }
    }
};


#endif //HHOS_SH_H
