//
// Created by 李新锐 on 20/05/2018.
//
#include "sh.h"

void sh::history_push(const char *buf)
{
    memmove(histroy[1], histroy[0], buf_size * history_term * sizeof(char));
    strcpy(histroy[0], buf);
}

int sh::exec(const sh::cmd &input_cmd)
{
    if (is_command(input_cmd, "ls") || is_command(input_cmd, "dir")) {
        printf("You have %d user programs intalled\n", prog_cnt);
        printf("Program Name\t\t\tSector Number\n");
        for (size_t i = 0; i < prog_cnt; ++i) {
            printf("%s\t\t\t\t\t%d\n", progs[i].name, progs[i].lba);
        }
    }
    else if (is_command(input_cmd, "cls") || is_command(input_cmd, "clear"))
    {
        sys_clear_screen();
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
    else if (is_command(input_cmd, "history"))
    {
        for(int i = 1; i < history_term; ++i)
        {
            printf("%s\n", histroy[i]);
        }
    }
    else if (is_command(input_cmd, "run3"))
    {

    }
    else if (is_command(input_cmd, "run2"))
    {

    }
    else if (is_command(input_cmd, "run1"))
    {
    }
    else if (is_command(input_cmd, "crash"))
    {
        int n = atoi(inputs[input_cmd.start + 1]);
        printf("%d\n", n);
        switch (n)
        {
        case 0: {asm volatile("int $0x0");}
        case 1: {asm volatile("int $0x1");}
        case 2: {asm volatile("int $0x2");}
        case 3: {asm volatile("int $0x3");}
        case 4: {asm volatile("int $0x4");}
        case 5: {asm volatile("int $0x5");}
        case 13:{asm volatile("int $0x13");}
        default:{asm volatile("int $0x13");}
        }

    }
    else if (is_command(input_cmd, "date"))
    {
        read_rtc();
        printf("%s\n", sys_internal_time_str);
    }
#ifdef IDE_TEST
    else if (is_command(input_cmd, "ide"))
    {
        ide_test();
    }
#endif
    else {
        bool found = false;
        for(size_t i = 0; i < prog_cnt; ++i)
        {
            if (is_command(input_cmd, progs[i].name))
            {
                found = true;
                bin_loader::load_binary_from_disk(SEL_USER_DATA0, progs[i].lba);
                bin_loader::exec(SEL_USER_CODE0, SEL_USER_DATA0);
                //if(!(strlen(progs[i].name) > 2 && progs[i].name[0] == 'c' && progs[i].name[1] == '_'))
                if(false)
                {
                    sys_clear_screen();
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

void sh::read_prog_record()
{
    sys_read_hard_disk(SEL_KERN_DATA, (uint32_t)record_buf, REC_FILE_SECTOR, 1);
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

void sh::read_help_file()
{
    sys_read_hard_disk(SEL_KERN_DATA, (uint32_t)help, HELP_FILE_SECTOR, 1);
}

int sh::split_input(char *buf)
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

int sh::split_batch(char **inputs, int input_cnt)
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
                cmds[i].cnt = 1;
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

int sh::bf(const char *Pattern, const char *Text) {
    using ss_t = int;
    ss_t m = strlen(Pattern);
    ss_t n = strlen(Text);
    ss_t i = 0, k = 0;
    while(k < m && i < n) {
        if(Pattern[k] == Text[i]) {
            ++k;
            ++i;
        }
        else {
            i = i - k + 1;
            k = 0;
        }
    }
    if(k >= m)
        return i - k;
    return -1;
}

sh::sh(){
    memset(buf, 0, buf_size);
    read_prog_record();
    read_help_file();
}

void sh::run() {
    printf("%s", prompt);
    while(true)
    {
        unsigned char in = getchar();
        if (in == KEY_UP)
        {
            printf("KEY UP PRESSED\n");
            continue;
        }
        if (in == KEY_DN)
        {
            printf("KEY DOWN PRESSED\n");
            continue;
        }
        if (in == C('C'))
        {
            printf("Ctrl-C PRESSED\n");
            continue;
        }
        if (in == C('D'))
        {
            printf("Ctrl-D PRESSED\n");
            continue;
        }
        if (in == '\b')
        {
            if(pos != 0){
                putchar('\b');
                buf[pos--] = '\0';
            }
        }
        else if (in == 13 || in == 10)
        {
            putchar('\n');
            if(strlen(buf) == 0)
            {
                printf("%s", prompt);
                continue;
            }
            history_push(buf);
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
        else if( in == '\t')
        {
            bool first_matching = false;
            for(int i = 0; i < supported_cmd_num; ++i){
                if(bf(buf, cmd_list[i]) != -1)
                {
                    if(!first_matching){
                        printf("\n");
                        first_matching = true;
                    }
                    printf("%s\n", cmd_list[i]);
                }
            }
            for(size_t i = 0; i < prog_cnt; ++i)
                if(bf(buf, progs[i].name) != -1)
                {
                    if(!first_matching){
                        printf("\n");
                        first_matching = true;
                    }
                    printf("%s\n", progs[i].name);
                }
            printf("%s", prompt);
            printf("%s", buf);
        }
        else {
            putchar(in);
            buf[pos++] = in;
        }
    }
}
