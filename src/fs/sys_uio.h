//
// Created by 李新锐 on 21/06/2018.
//

#ifndef SRC_SYS_STDIO_H
#define SRC_SYS_STDIO_H

#include "../include/defines.h"
#include "../libc/sys/stat.h"
#define MAX_FD_NUM 256
#define SEEK_SET 0
#define SEEK_CUR 1
#define O_CREAT 1

void fs_init();
int sys_fstat(int fildes, stat *buf);
int sys_open(const char *path, uint32_t mode);
int sys_read(int fildes, void *buf, size_t nbyte);
int sys_write(int fildes, void *buf, size_t nbyte);
int sys_close(int fd);
int sys_lseek(int fildes, int offset, int whence);
int sys_ls(int fd);

#endif //SRC_SYS_STDIO_H
