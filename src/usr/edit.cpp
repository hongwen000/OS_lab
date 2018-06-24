//
// Created by 李新锐 on 23/06/2018.
//

#include "../libc/sys/hhos.h"
#include "../libc/stdio.h"
#include "../libc/string.h"
#include "../fs/sys_uio.h"
#include "../fs/fat32.h"

#define KEY_LF          0xE4
#define KEY_RT          0xE5
#define MAX_BUF         512
char buf[MAX_BUF];
static char dir_buf[0x20];
static char LongFileNameBuffer[LONG_FILENAME_MAX_LEN];
static FileInfo_t FS_read_one_file_info(DirectoryEntry_t *dir_entry);
extern void main()
{
    puts("Please enter the absolute path of file: ");
    char fn [13];
    int l_fn = 0;
    scanf("%s", fn);
    int fd;
    fd = open(fn, 1);
    if(fd == -1)
    {
        puts("Can not open or create file\n");
        return;
    }
    size_t pos = 0;
    int ch;
    stat fst;
    fstat(fd, &fst);
    size_t sz = fst.size;
    printf("\n--------------------------------------------------------------\n");
    read(fd, buf, sz);
    puts(buf);
    for(size_t i = 0; i < sz; ++i)
        putchar(TTY_MOVE_CURSOR_BACK);
    while (true)
    {
        ch = getchar();
        if(ch == 27)
            break;
        else if(ch == KEY_LF)
        {
            if(pos > 0)
            {
                pos--;
                putchar(TTY_MOVE_CURSOR_BACK);
            }
        }
        else if(ch == KEY_RT)
        {
            if(pos < sz)
            {
                pos++;
                putchar(TTY_MOVE_CURSOR_FORWARD);
            }
        }
        else if (ch == '\b')
        {
            if(pos)
            {
                pos--;
                sz--;
                memmove(buf+pos, buf+pos + 1, MAX_BUF - pos);
                putchar('\b');
                for(size_t i = pos; i < sz; ++i)
                {
                    putchar(buf[i]);
                }
                putchar(' ');
                for(size_t i = pos; i <= sz; ++i)
                    putchar(TTY_MOVE_CURSOR_BACK);
            }
        }
        else
        {
            if(pos == sz)
            {
                buf[pos++] = ch;
                sz++;
                putchar(ch);
            }
            else
            {
                buf[pos++] = ch;
                putchar(ch);
            }
        }
    }
    printf("\n--------------------------------------------------------------\n");
    printf("Saving file...\n");
    lseek(fd, 0, SEEK_SET);
    write(fd, buf, sz);
    close(fd);
    const char *name;
    const char *ext;
    name = strrchr(fn, '/');
    name = name + 1;
    ext = strrchr(fn, '.');
    size_t ext_len;
    size_t name_len;
    if(ext == nullptr)
    {
        ext_len = 0;
        name_len = strlen(name);
    }
    else
    {
        ext = ext + 1;
        ext_len = strlen(ext);
        name_len = strlen(name) - ext_len - 1;
    }
    char parent[strlen(fn)];
    strcpy(parent, fn);
    parent[name-fn] = 0;
    int fd_parent;
    fd_parent = open(parent, 0);
    if(fd_parent == -1)
    {
        puts("Can not change file size");
        return;
    }
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
                if(strcmp(file_info.filename, name) == 0)
                {
                    lseek(fd, -4, SEEK_CUR);
                    write(fd, &sz, 4);
                    close(fd);
                    break;
                }
                else
                {
                }
            }
        }
    }while(dir_buf[0] != '\x00');
}

static char formated_file_name[16];
static uint8_t cp_buf[512];
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