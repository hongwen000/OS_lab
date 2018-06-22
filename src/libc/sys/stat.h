//
// Created by 李新锐 on 22/06/2018.
//

#ifndef SRC_STAT_H
#define SRC_STAT_H

#include "../../include/defines.h"
#define S_IFMT  0170000    /* type of file */
#define S_IFIFO 0010000    /* named pipe */
#define S_IFCHR 0020000    /* character special */
#define S_IFDIR 0040000    /* directory */
#define S_IFBLK 0060000    /* block special */
#define S_IFREG 0100000    /* regular */
#define S_IFLNK 0120000    /* symbolic link (Minix-vmd) */
struct stat {
    uint16_t dev;
    uint16_t cluster;
    uint16_t mode;
    uint32_t size;
};
#endif //SRC_STAT_H
