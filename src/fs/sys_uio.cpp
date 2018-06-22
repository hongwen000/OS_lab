//
// Created by 李新锐 on 21/06/2018.
//


#include "sys_uio.h"
#include "fat32.h"
#include "../libc/stdio.h"

FileInfo_t fd[MAX_FD_NUM];
void fs_init()
{
    fs_global_info = BPB_read(fs_global_buf.data);
    fs_start_sector = 0;
    Show_FS_Info(fs_global_info);
}

int sys_fstat(int fildes, stat *buf)
{
    buf->size = fd[fildes].FileSize;
    return 0;
}

int sys_open(const char *path, uint32_t mode)
{
    debug_printf("sys_open: %s, mode: %u", path, mode);
    for (int i = 0; i < MAX_FD_NUM; ++i)
    {
        if(!fd[i].used)
        {
            auto ret = FS_fopen(path, "rw", &fd[i], fs_global_info, &fs_global_buf);
            //Open fail but to create
            if(ret == nullptr)
            {
                if(mode & O_CREAT)
                {
                    FS_touch(path, fs_global_info, &fs_global_buf);
                    ret = FS_fopen(path, "rw", &fd[i], fs_global_info, &fs_global_buf);
                    if(ret == nullptr)
                        return -1;
                    printf("File %s created\n", path);
                    printf("Parent Directory at Cluster: %u\n", ret->ParentStartCluster);
                    printf("File Start at Cluster: %u\n", ret->StartCluster);
                    printf("File size (byte): %u\n", ret->FileSize);
                    return i;
                }
                //Create fail
                else
                {
                    return -1;
                }
            }
            //Open success
            else
            {
                if(ret->FileSize == 4294967295)
                {
                    printf("Directory %s opened\n", path);
                }
                else
                {
                    printf("file %s opened\n", path);
                    printf("Parent Directory at Cluster: %u\n", ret->ParentStartCluster);
                    printf("File Start at Cluster: %u\n", ret->StartCluster);
                    printf("File size (byte): %u\n", ret->FileSize);
                }
                fd[i].used = true;
                return i;
            }
        }
    }
    return -1;
}
int sys_read(int fildes, void *buf, size_t nbyte)
{
    return FS_fread((uint8_t*)buf, nbyte, &fd[fildes], fs_global_info, &fs_global_buf);
}
int sys_write(int fildes, void *buf, size_t nbyte)
{
    return FS_fwrite((uint8_t*)buf, nbyte, 1, &fd[fildes], fs_global_info, &fs_global_buf);
}
int sys_close(int fildes)
{
    fd[fildes].used = false;
    return FS_fclose(&fd[fildes], &fs_global_buf);
}

int sys_lseek(int fildes, int offset, int whence)
{
    int ori;
    if(whence == SEEK_SET)
        ori = 0;
    else if (whence == SEEK_CUR)
        ori = fd[fildes].pos;
    else
        ori = fd[fildes].FileSize;
    return FS_fseek(&fd[fildes], ori, offset, fs_global_info, &fs_global_buf);
}
