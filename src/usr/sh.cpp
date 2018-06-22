//
// Created by 李新锐 on 20/05/2018.
//
#include "sh.h"
#include "../fs/fat32.h"
#include "../fs/sys_uio.h"

void sh::history_push(const char *buf)
{
    memmove(histroy[1], histroy[0], buf_size * history_term * sizeof(char));
    strcpy(histroy[0], buf);
}
static char formated_file_name[16];
static char *FS_format_file_name(DirectoryEntry_t *entry) {
    int i, j;
    uint8_t *entryname = entry->Filename;
//    uint8_t *formated_file_name=new uint8_t [16];
    if (entry->Attributes != 0x0f) {
        j = 0;
        for (i = 0; i<8; i++) {
            if (entryname[i] != ' ') {
                formated_file_name[j++] = entryname[i];
            }
        }
        if(entryname[8] != ' ')
            formated_file_name[j++] = '.';
        for (i = 8; i<11; i++) {
            if (entryname[i] != ' ') {
                formated_file_name[j++] = entryname[i];
            }
        }
        formated_file_name[j++] = '\x00';
    }
    else {
        LongFileName_t *LongEntry = (LongFileName_t*)entry;
        j = 0;
        for (i = 0; i<5; i++) {
            formated_file_name[j++] = (uint8_t)LongEntry->name_1[i];
        }
        for (i = 0; i<6; i++) {
            formated_file_name[j++] = (uint8_t)LongEntry->name_2[i];
        }
        for (i = 0; i<2; i++) {
            formated_file_name[j++] = (uint8_t)LongEntry->name_3[i];
        }
        formated_file_name[j++] = '\x00';
    }

    return (char *)formated_file_name;
}
static FileInfo_t FS_read_one_file_info(DirectoryEntry_t *dir_entry)
{
    constexpr int TO_DO_SECTOR_PER_CLUSTER = 1;
    FileInfo_t ret;
    if ((dir_entry->Attributes &ATTR_LONG_NAME_MASK) == ATTR_LONG_NAME)
    {
        LongFileName_t *LongFileName = (LongFileName_t*)dir_entry;
        strcpy(ret.LongFilename, FS_format_file_name(dir_entry));
        ret.attributes = ATTR_LONG_NAME;
        ret.checksum = LongFileName->checksum;
    }
    else
    {
        strcpy(ret.filename, "");
        strcpy(ret.filename, FS_format_file_name(dir_entry));
        ret.attributes = dir_entry->Attributes;
        ret.CurrentByte = 0;
        ret.StartCluster = ret.CurrentCluster = (((uint32_t)dir_entry->ClusterLow) << 16) | dir_entry->FirstCluster;
        ret.flags = 0;
        ret.mode = ENRTRY_MODE_WRITE;
        ret.pos = 0;
        ret.FileSize = dir_entry->FileSize;
        if ((ret.attributes & ATTR_DIRECTORY) == 1)
        {
            ret.FileSize = SECTOR_SIZE * TO_DO_SECTOR_PER_CLUSTER;
        }
        ret.CurrentClusterOffset = 0;
    }
    return ret;
}

static char dir_buf[0x20];
static char LongFileNameBuffer[LONG_FILENAME_MAX_LEN];
static uint32_t sz;
static char fmt[] = "%u\n";
int sh::sh_exec(const sh::cmd &input_cmd)
{
    if (is_command(input_cmd, "ls") || is_command(input_cmd, "dir")) {
        int fd;
        if(input_cmd.cnt == 1)
            fd = open(pwd, 0);
        else
            fd = open(inputs[input_cmd.start + 1], 0);
        if(fd == -1)
            return FAIL;
        LongFileNameBuffer[0] = 0;
        int n = 0;
        do
        {
            read(fd, dir_buf, 0x20);
            DirectoryEntry_t *CurrentOneFileInfo = (DirectoryEntry_t *)&dir_buf;
            auto file_info=FS_read_one_file_info(CurrentOneFileInfo);
            if ((file_info.attributes & 0x0f) == 0x0f)
            {
                strcpy(LongFileNameBuffer + strlen(LongFileNameBuffer), file_info.LongFilename);
            }
            else
            {
                strcpy(file_info.LongFilename,LongFileNameBuffer);
                //Got one file
                LongFileNameBuffer[0] = 0;
                if(file_info.filename[0] == '\000' || file_info.filename[0] == 0x7f || file_info.filename[0] == 0xe5)
                {
                    continue;
                }
                else
                {
                    for(int i = 0; i < 8; ++i)
                    {
                        putchar(file_info.filename[i]);
                    }
//                    putchar('\n');
                    sz =file_info.FileSize;
//                    bochs_break();
                    printf(fmt, sz);
//                    printf("Here 5\n");
//                    printf("");
                }
            }
        }while(dir_buf[0] != '\x00');
    }
    else if (is_command(input_cmd, "mv") || is_command(input_cmd, "move"))
    {
        if(input_cmd.cnt != 3)
        {
            printf("mv [SOURCE FILE] [DESTINATION FILE]\n");
            return FAIL;
        }
        int fd;
        fd = open(pwd, 0);
        if(fd == -1)
            return FAIL;
        LongFileNameBuffer[0] = 0;
        do
        {
            read(fd, dir_buf, 0x20);
            DirectoryEntry_t *CurrentOneFileInfo = (DirectoryEntry_t *)&dir_buf;
            auto file_info=FS_read_one_file_info(CurrentOneFileInfo);
            if ((file_info.attributes & 0x0f) == 0x0f)
            {
                strcpy(LongFileNameBuffer + strlen(LongFileNameBuffer), file_info.LongFilename);
            }
            else
            {
                strcpy(file_info.LongFilename,LongFileNameBuffer);
                //Got one file
                LongFileNameBuffer[0] = 0;
                if(file_info.filename[0] == '\000' || file_info.filename[0] == 0x7f || file_info.filename[0] == 0xe5)
                {
                    continue;
                }
                else
                {
                    if(strcmp(file_info.filename, inputs[input_cmd.start + 1]) == 0)
                    {
                        lseek(fd, -0x20, SEEK_CUR);
                        uint8_t ch;
                        for(size_t i = 0; i < 8; ++i)
                        {
                            auto fn = inputs[input_cmd.start+1];
                            if(i < strlen(fn))
                            {
                                ch = fn[i];
                            }
                            else
                            {
                                ch = ' ';
                            }
                            write(fd, &ch, 1);
                        }
                        close(fd);
                        printf("rm: succeeded\n");
                        break;
                    }
                }
            }
        }while(dir_buf[0] != '\x00');
    }
    else if (is_command(input_cmd, "rm") || is_command(input_cmd, "del"))
    {
        if(input_cmd.cnt != 2)
        {
            printf("del [FILE]\n");
            return FAIL;
        }
        int fd;
        fd = open(pwd, 0);
        if(fd == -1)
            return FAIL;
        LongFileNameBuffer[0] = 0;
        do
        {
            read(fd, dir_buf, 0x20);
            DirectoryEntry_t *CurrentOneFileInfo = (DirectoryEntry_t *)&dir_buf;
            auto file_info=FS_read_one_file_info(CurrentOneFileInfo);
            if ((file_info.attributes & 0x0f) == 0x0f)
            {
                strcpy(LongFileNameBuffer + strlen(LongFileNameBuffer), file_info.LongFilename);
            }
            else
            {
                strcpy(file_info.LongFilename,LongFileNameBuffer);
                //Got one file
                LongFileNameBuffer[0] = 0;
                if(file_info.filename[0] == '\000' || file_info.filename[0] == 0x7f || file_info.filename[0] == 0xe5)
                {
                    continue;
                }
                else
                {
                    if(strcmp(file_info.filename, inputs[input_cmd.start + 1]) == 0)
                    {
                        lseek(fd, -0x20, SEEK_CUR);
                        uint8_t ch = 0xe5;
                        write(fd, &ch, 1);
                        close(fd);
                        printf("rm: succeeded\n");
                        break;
                    }
                }
            }
        }while(dir_buf[0] != '\x00');
    }
    else if (is_command(input_cmd, "cp") || is_command(input_cmd, "copy"))
    {
        if(input_cmd.cnt != 3)
        {
            printf("cp [SOURCE FILE] [DESTINATION FILE]\n");
            return FAIL;
        }
        auto src = inputs[input_cmd.start + 1];
        auto dst = inputs[input_cmd.start + 2];
        if(strcmp(src, dst) == 0)
        {
            printf("cp: Source file name can not be same to dest file name\n");
            return FAIL;
        }
        int fd_s = open(src, 0);
        if(fd_s == -1)
        {
            printf("cp: Can not open source file\n");
            return FAIL;
        }
        int fd_d = open(src, 1);
        if(fd_d == -1)
        {
            printf("cp: Can not open dest file\n");
            return FAIL;
        }
        stat st;
        fstat(fd_s, &st);
        auto sz = st.size;
        printf("cp: Ready to copy %u bytes\n", sz);
        uint8_t ch;
        for(size_t i = 0; i < sz; ++i)
        {
            read(fd_s, &ch, 1);
            write(fd_d, &ch, 1);
        }
        close(fd_s);
        close(fd_d);
    }
    else if (is_command(input_cmd, "cls") || is_command(input_cmd, "clear"))
    {
        asm volatile("mov $28, %ah\n\tint $0x98");
        asm volatile("mov $29, %ah\n\tint $0x98");
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
//    else if (is_command(input_cmd, "date"))
//    {
//        read_rtc();
//        printf("%s\n", sys_internal_time_str);
//    }
#ifdef IDE_TEST
    else if (is_command(input_cmd, "ide"))
    {
        ide_test();
    }
#endif
    else {
        bool found = false;
//        for(size_t i = 0; i < prog_cnt; ++i)
//        {
//            if (is_command(input_cmd, progs[i].name))
//            {
                found = true;
                int n = fork();
                if (n == 0)
                {
                     exec(inputs[input_cmd.start]);
//                    exec((uint32_t)progs[i].lba);
                }
                else {
                    printf("---------------------------------------\n");
                    printf("Shell forked for user program, pid = %d\n", n);
                    printf("---------------------------------------\n");
                    wait();
                    printf("---------------------------------------\n");
                    printf("user program exited\n");
                    printf("---------------------------------------\n");
                }
//                break;
//            }
//        }
        if (!found)
            printf("%s\n", "No such command or file");
    }
    return SUCCESS;
}

void sh::read_prog_record()
{
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
    printf("%s%s>", prompt, pwd);
    while(true)
    {
//        printf("");
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
                printf("%s%s>", prompt, pwd);
                continue;
            }
            history_push(buf);
            int input_cnt = split_input(buf);
            if (input_cnt == 0)
                continue;
            int cmd_cnt = split_batch(inputs, input_cnt);
            for(int cmd_num = 0; cmd_num < cmd_cnt; ++ cmd_num)
            {
                sh_exec(cmds[cmd_num]);
            }
            memset(buf, 0, buf_size);
            pos = 0;
            printf("%s%s>", prompt, pwd);
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
            printf("%s%s>", prompt, pwd);
            printf("%s", buf);
        }
        else {
            putchar(in);
            buf[pos++] = in;
        }
    }
}

extern "C" void main()
{
    printf("--------------------------------------\n");
    printf("Shell loaded, system ready for use\n");
    printf("--------------------------------------\n");
    sh sh1;
    sh1.run();
}