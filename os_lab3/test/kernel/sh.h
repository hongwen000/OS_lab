//
// Created by 李新锐 on 31/03/2018.
//

#ifndef HHOS_SH_H
#define HHOS_SH_H

#include "../libc/stdio.h"
#include "../libc/string.h"
#include "../libc/stdlib.h"
#include "sys_lib.h"
#include "../basic_lib/sys_lib.h"
#include "bin_loader.h"
#include "../libc/sys/hhos.h"
#include "../libc/ctype.h"
#define HELP_FILE_SECTOR 32
#define REC_FILE_SECTOR 52

class sh{
private:
    static constexpr int buf_size = 128;
    static constexpr int SUCCESS = 0;
    char * inputs[buf_size / 2];
    struct cmd{
        int start;
        int cnt = 0;
    };
    cmd cmds[buf_size / 4];
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
    bool inline is_command(const cmd & input_cmd, const char* cmd_name){
        return (strcmp(inputs[input_cmd.start], cmd_name) == 0);
    }
    int exec(const cmd & input_cmd)
    {
        if (is_command(input_cmd, "ls") || is_command(input_cmd, "dir")) {
            printf("You have %d user programs intalled\n\n", prog_cnt);
            for (size_t i = 0; i < prog_cnt; ++i) {
                printf("%s\n", progs[i].name);
            }
        }
        else if (is_command(input_cmd, "cls") || is_command(input_cmd, "clear"))
        {
            sys_bios_clear_screen();
            sys_get_current_tty()->tty_init();
        }
        else if (is_command(input_cmd, "help"))
        {
            printf("%s\n", help);

        }
        else if (is_command(input_cmd, "echo"))
        {
            for(int i = 1; i < input_cmd.cnt; ++i)
            {
                printf("%s ", inputs[input_cmd.start + i]);
            }
            printf("\n");
        }
        else {
            bool found = false;
            for(size_t i = 0; i < prog_cnt; ++i)
            {
                if (is_command(input_cmd, progs[i].name))
                {
                    found = true;
                    bin_loader::load_binary_from_floppy(progs[i].lba);
                    if(!(strlen(progs[i].name) > 2 && progs[i].name[0] == 'c' && progs[i].name[1] == '_'))
                    {
                        sys_bios_clear_screen();
                        sys_get_current_tty()->tty_init();
                    }
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
        sys_read_disk(0, (uint32_t)record_buf, REC_FILE_SECTOR, 1);
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
        sys_read_disk(0, (uint32_t)help, HELP_FILE_SECTOR, 1);
    }
    int split_input(char* buf)
    {
        int i = 0;
        char * p = buf;
        while(*p && isspace(*p)){
            ++p;
        };
        bool con = false;
        while (*p)
        {
            if(isspace(*p))
            {
                *p = '\0';
                if(con)
                {
                    con = false;
                    ++i;
                }
            }
            else
            {
                if(!con)
                {
                    inputs[i] = p;
                    con = true;
                }
            }
            ++p;
        }
        if(con) ++i;
        return i;
    }
    int split_batch(char** inputs, int input_cnt)
    {
        int i = 0;
        int p = 0;
        while(strcmp(inputs[p], ";") == 0){
            ++p;
        };
        bool con = false;
        while (p < input_cnt)
        {
            if(strcmp(inputs[p], ";") == 0)
            {

                if(con)
                {
                    con = false;
                    ++i;
                }
            }
            else
            {
                if(!con)
                {
                    cmds[i].start = p;
                    cmds[i].cnt++;
                    con = true;
                }
                else {
                    cmds[i].cnt++;
                }
            }
            ++p;
        }
        if(con) ++i;
        return i;
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
                int input_cnt = split_input(buf);
                if (input_cnt == 0)
                    continue;
                int cmd_cnt = split_batch(inputs, input_cnt);
                for(int cmd_num = 0; cmd_num < cmd_cnt; ++ cmd_num)
                {
                    exec(cmds[cmd_num]);
                }
                memset(buf, 0, buf_size);
                pos = 0;
                printf("%s", prompt);
            }
            else if (in == 0)
            {
                continue;
            }
            else {
                putchar(in);
                buf[pos++] = in;
            }
        }
    }
};


#endif //HHOS_SH_H
