//
// Created by 李新锐 on 20/06/2018.
//

#include "../include/defines.h"
#include "../libc/string.h"
#include "../kernel_lib/debug_printf.h"

#ifndef SRC_FAT32_H
#define SRC_FAT32_H

#define FAT32_DEBUG
#ifdef FAT32_DEBUG
#define DBG_PRINTF(...) debug_printf(__VA_ARGS__)
#else
#define DBG_PRINTF(...)
#endif // DEBUG

#define LONG_FILENAME_MAX_LEN 32
#define FS_MAX_PATH 255
#define FAT_MASK_EOC 0x0FFFFFFF
#define SECTOR_SIZE 512u
#define SECTOR_ALREADY_IN_MEMORY 0x00
#define NO_ERROR 0x00
#define ERROR_OPEN_VHD 0x01
#define ERROR_FS_TYPE 0x02
#define ERROR_SECTOR_SIZE 0x04
#define ERROR_SEEK_FILE 0x08
#define ERROR_ILLEGAL_PATH 0x0F
#define ERROR_READ_FAT 0x1F
#define ERROR_WRITE_PROTECT 0x2F
#define ERROR_VOLUME_FULL 0x4F
#define ERROR_UNKOWN 0xFFFFFFFF
#define CURRENT_FLAG_CLEAN 0x00
#define CURRENT_FLAG_DIRTY 0x01

#define ENRTRY_FLAG_DIRTY 0x01
#define ENRTRY_FLAG_OPEN 0x02
#define ENRTRY_FLAG_SIZECHANGED 0x04
#define ENRTRY_FLAG_ROOT 0x08

#define ENRTRY_MODE_READ 0x01
#define ENRTRY_MODE_WRITE 0x02
#define ENRTRY_MODE_APPEND 0x04
#define ENRTRY_MODE_OVERWRITE 0x08

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_DEVICE 0x40
#define ATTR_UNUSED 0x80
#define ATTR_LONG_NAME (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)
#define ATTR_LONG_NAME_MASK (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID | ATTR_DIRECTORY | ATTR_ARCHIVE)
#define LAST_LONG_ENTRY_MASK 0x40
#define _IO_SKIPWS 01
#define _IO_LEFT 02
#define _IO_RIGHT 04
#define _IO_INTERNAL 010
#define _IO_DEC 020
#define _IO_OCT 040
#define _IO_HEX 0100
#define _IO_SHOWBASE 0200
#define _IO_SHOWPOINT 0400
#define _IO_UPPERCASE 01000
#define _IO_SHOWPOS 02000
#define _IO_SCIENTIFIC 04000
#define _IO_FIXED 010000
#define _IO_UNITBUF 020000
#define _IO_STDIO 040000
#define _IO_DONT_CLOSE 0100000
#endif //SRC_FAT32_H

struct PartitionTable_t {
    uint8_t MBR_BootIndicator;
    uint8_t MBR_StartHead[3];
    uint8_t MBR_PartionType;
    uint8_t MBR_EndHead[3];
    uint32_t MBR_SectorsPreceding;
    uint32_t MBR_SectorsInPartition;
}__attribute__ ((__packed__));

struct MBR_t {
    uint8_t MBR_BootRecord[446];
    PartitionTable_t MBR_PartitionTable[4];
    uint16_t MBR_Signature;
}__attribute__ ((__packed__));

struct MBR_Info_t {
    bool Active;
    uint32_t StartCylinder;
    uint32_t StartHead;
    uint32_t StartSector;
    uint32_t ReservedSector;
    char FS_Type[8];
};

struct BPB_t {
    uint8_t     BS_JumpBoot[3];
    uint8_t     BS_OEMName[8];
    uint16_t    BytesPerSector;
    uint8_t     SectorsPerCluster;
    uint16_t    ReservedSectorCount;
    uint8_t     NumFATs;
    uint16_t    RootEntryCount;
    uint16_t    TotalSectors16;
    uint8_t     Media;
    uint16_t    FATSize16;
    uint16_t    SectorsPerTrack;
    uint16_t    NumberOfHeads;
    uint32_t    HiddenSectors;
    uint32_t    TotalSectors32;
    uint32_t    FATSize;
    uint16_t    ExtFlags;
    uint16_t    FSVersion;
    uint32_t    RootCluster;
    uint16_t    FSInfo;
    uint16_t    BkBootSec;
    uint8_t     Reserved[12];
    uint8_t     BS_DriveNumber;
    uint8_t     BS_Reserved1;
    uint8_t     BS_BootSig;
    uint32_t    BS_VolumeID;
    uint8_t     BS_VolumeLabel[11];
    uint8_t     BS_FileSystemType[8];
} __attribute__ ((__packed__));

struct FS_Info_t {
    char Label[11];
    uint8_t SectorsPerCluster;
    uint32_t ReservedSectors;
    uint32_t DataStartSector;
    uint32_t DataSectors;
    uint32_t DataClusters;
    uint32_t TotalSectors;
    uint32_t FatSize;
    uint32_t RootDirSectors;
};

struct DirectoryEntry_t {
    uint8_t Filename[8];
    uint8_t Extension[3];
    uint8_t Attributes;
    uint8_t Reserved;
    uint8_t CreationTimeMs;
    uint16_t CreationTime;
    uint16_t CreationDate;
    uint16_t LastAccessTime;
    uint16_t ClusterLow;
    uint16_t ModifiedTime;
    uint16_t ModifiedDate;
    uint16_t FirstCluster;
    uint32_t FileSize;
} __attribute__((__packed__));

struct LongFileName_t {
    uint8_t sequence_number;
    uint16_t name_1[5];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t checksum;
    uint16_t name_2[6];
    uint16_t firstCluster;
    uint16_t name_3[2];
} __attribute__((__packed__));

struct FileInfo_t {
    uint32_t ParentStartCluster;
    uint32_t StartCluster;
    uint32_t CurrentClusterOffset;
    uint32_t CurrentCluster;
    short CurrentSector;
    short CurrentByte = 0;
    uint32_t pos = 0;
    uint8_t flags;
    uint8_t attributes;
    uint8_t mode;
    uint32_t FileSize;
    char filename[16] = "";
    char LongFilename[LONG_FILENAME_MAX_LEN] = "";
    uint8_t checksum;
    bool used = false;
};

struct SectorBuffer_t {
    uint32_t SectorNumber;
    uint8_t SectorFlags;
    uint8_t data[512];
};

struct Path_t {
    char DirectoryName[FS_MAX_PATH];
    uint32_t cluster;
};

FS_Info_t FS_read_info(uint8_t* buffer);
void Show_FS_Info(FS_Info_t fs_info);
void Read_Print_File(const char *filename, FileInfo_t* fp,FS_Info_t fs_info, SectorBuffer_t* buffer);
FS_Info_t BPB_read(uint8_t * buffer);
extern SectorBuffer_t fs_global_buf;
extern uint32_t fs_start_sector;
extern FS_Info_t fs_global_info;
FileInfo_t * FS_fopen(const char *filename, const char *mode, FileInfo_t* fp, FS_Info_t fs_info, SectorBuffer_t* buffer);
int FS_fread(uint8_t *dest, size_t size, FileInfo_t *fp, FS_Info_t fs_info, SectorBuffer_t *buffer);
int FS_fwrite(uint8_t *src, uint32_t size, uint32_t count, FileInfo_t *fp, FS_Info_t fs_info, SectorBuffer_t* buffer);
int FS_fclose(FileInfo_t *fp, SectorBuffer_t* buffer);
int FS_fseek(FileInfo_t *fp, int32_t base, long offset, FS_Info_t fs_info, SectorBuffer_t* buffer);
int FS_touch(const char *filename, FS_Info_t fs_info, SectorBuffer_t* buffer);
