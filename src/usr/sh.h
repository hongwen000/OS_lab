//
// Created by 李新锐 on 31/03/2018.
//

#ifndef HHOS_SH_H
#define HHOS_SH_H

#include "../include/defines.h"
#include "../libc/stdio.h"
#include "../libc/string.h"
#include "../libc/stdlib.h"
#include "../libc/sys/hhos.h"
#include "../libc/ctype.h"
#include "../driver/kb.h"
#define HELP_FILE_SECTOR 64
#define REC_FILE_SECTOR 128

class sh{
private:
    static constexpr int buf_size = 128;
    static constexpr int history_term = 10;
    static constexpr char * prompt = "HHOS> ";
    static constexpr int SUCCESS = 0;
    static constexpr char * cmd_list[] = {
            "ls",
            "dir",
            "cls",
            "clear",
            "help",
            "echo",
            "history",
            "date"
    };
    int supported_cmd_num = 8;

    char * inputs[buf_size / 2];
    struct cmd{
        int start;
        int cnt = 0;
    };
    cmd cmds[buf_size / 4];
    char buf[buf_size];
    char histroy[history_term][buf_size];
    char record_buf[512] =
            "11\n"
            "bc\n"
            "192\n"
            "sleep\n"
            "256\n"
            "test\n"
            "320\n"
            "usr1\n"
            "384\n"
            "usr2\n"
            "448\n"
            "usr3\n"
            "512\n"
            "usr4\n"
            "576\n"
            "asm1\n"
            "640\n"
            "asm2\n"
            "704\n"
            "asm3\n"
            "768\n"
            "asm4\n"
            "832\n";
    char help[512] =
            "HHOS shell 1.1.0 help\n"
            "Command    Usage\n"
            "ls         list all user programs\n"
            "dir        same to ls\n"
            "cls        clear screen\n"
            "clear      same to cls\n"
            "echo       echo your input\n"
            "help       show help\n"
            "\n"
            "Enter the name of a user program to execuate it\n"
            "You can run multiply commands by spliting them \n"
            "using  ;  (must have spaces in both sides)";

    size_t prog_cnt = 0;
    size_t pos = 0;

    bool inline is_command(const cmd & input_cmd, const char* cmd_name){
        return (strcmp(inputs[input_cmd.start], cmd_name) == 0);
    }
    struct prog_entry {
        char name[32];
        int lba;
    };
    prog_entry progs[16];

    void history_push(const char* buf);

    int sh_exec(const cmd & input_cmd);
    void read_prog_record();
    void read_help_file();
    int split_input(char* buf);
    int split_batch(char** inputs, int input_cnt);

    int bf(const char* Pattern, const char * Text);


public:
    sh();
    void run ();
};


#endif //HHOS_SH_H
