//
// Created by 李新锐 on 20/06/2018.
//

#include "../include/defines.h"
#include "fat32.h"
#include "../kernel_lib/debug_printf.h"
#include "../libc/stdio.h"
SectorBuffer_t fs_global_buf;
uint32_t fs_start_sector;
FS_Info_t fs_global_info;
int read_sector(uint8_t * buffer, uint32_t sector)
{
    sys_read_hard_disk(SEL_KERN_DATA, (uint32_t)buffer, sector, 1);
    return 0;
}
int write_sector(uint8_t * buffer, uint32_t sector)
{
    sys_write_hard_disk(SEL_KERN_DATA, (uint32_t)buffer, sector, 1);
    return 0;
}
int FS_read_sector(uint8_t * buffer, uint32_t sector)
{
    return read_sector(buffer,sector + fs_start_sector);
}
MBR_Info_t MBR_read(uint8_t * buffer)
{
    DBG_PRINTF("MBR_read:Start reading MBR\n");
    read_sector(buffer, 0);
    MBR_t *MBR;
    MBR = (MBR_t *)buffer;
    MBR_Info_t ret;
    ret.Active = (MBR->MBR_PartitionTable->MBR_BootIndicator) != 0;
    strcpy(ret.FS_Type, (MBR->MBR_PartitionTable->MBR_PartionType == 0x0c) ? "FAT32" : "UNKNOWN");
    ret.ReservedSector = MBR->MBR_PartitionTable->MBR_SectorsPreceding;
    fs_start_sector = ret.ReservedSector;
    return ret;
}

FS_Info_t BPB_read(uint8_t * buffer)
{
    DBG_PRINTF("BPB_read: Start reading BPB\n");
    FS_read_sector(buffer, 0);
    BPB_t *BPB;
    BPB = (BPB_t *)buffer;
    if (BPB->BytesPerSector != 512)
    {
        DBG_PRINTF("BPB_read: Wrong sector size %d\n", BPB->BytesPerSector);
    }
    FS_Info_t fs_info;
    strncpy(fs_info.Label,(char*) BPB->BS_VolumeLabel, 11);
    fs_info.FatSize = BPB->FATSize;
    fs_info.RootDirSectors = 0;
    fs_info.TotalSectors =BPB->TotalSectors32;
    fs_info.DataSectors = fs_info.TotalSectors - (BPB->ReservedSectorCount + (BPB->NumFATs * fs_info.FatSize) + fs_info.RootDirSectors);
    fs_info.SectorsPerCluster = BPB->SectorsPerCluster;
    if(fs_info.SectorsPerCluster != 0)
        fs_info.DataClusters = fs_info.DataSectors / fs_info.SectorsPerCluster;
    fs_info.ReservedSectors = BPB->ReservedSectorCount;
    fs_info.DataStartSector = BPB->ReservedSectorCount + (BPB->NumFATs * fs_info.FatSize) + fs_info.RootDirSectors;
    return fs_info;
}

FS_Info_t FS_read_info(uint8_t* buffer)
{
#ifdef USE_VHD
    auto mbr_info = MBR_read(buffer);
    if (mbr_info.FS_Type != "FAT32") {
        DBG_PRINTF("FS_inital: ERROR_FS_TYPE %s\n", mbr_info.FS_Type);
        return ERROR_FS_TYPE;
    }
#endif
    MBR_Info_t mbr_info;
    mbr_info.Active = 0;
    strcpy(mbr_info.FS_Type, "FAT32");
    mbr_info.ReservedSector = 0;
    mbr_info.StartCylinder = 0;
    mbr_info.StartHead = 0;
    mbr_info.StartHead = 0;
    mbr_info.StartSector = 0;
    fs_start_sector = 0;
    auto fs_info = BPB_read(buffer);
    return fs_info;
}
int FS_fsync(SectorBuffer_t* buffer)
{
    return write_sector(buffer->data, fs_start_sector+buffer->SectorNumber);
}
int FS_fetch(uint32_t sector, SectorBuffer_t* buffer)
{
    uint32_t ErrorLevel = 0;
    if (buffer->SectorNumber == sector)
    {
//        DBG_PRINTF("FS_fetch: sector already read\n");
        ErrorLevel|= SECTOR_ALREADY_IN_MEMORY;
        return ErrorLevel;
    }
    if (buffer->SectorFlags & CURRENT_FLAG_DIRTY)
    {
        DBG_PRINTF("FS_fetch: buffer is dirty, write back\n");
        ErrorLevel |= FS_fsync(buffer);
    }
    DBG_PRINTF("FS_fetch: read fresh sector%d\n", sector);
    ErrorLevel |= FS_read_sector(buffer->data, sector);
    buffer->SectorNumber = sector;
    return ErrorLevel;
}

uint32_t FS_get_FAT_entry(uint32_t cluster, FS_Info_t fs_info, SectorBuffer_t* buffer)
{
    uint32_t offset = 4 * cluster;
    uint32_t sector = fs_info.ReservedSectors + (offset/512);
    FS_fetch(sector, buffer);
    uint32_t *FAT_entry = ((uint32_t *) &(buffer->data[offset % 512]));
    DBG_PRINTF("FS_get_FAT_entry: Got FAT entry of cluster %d(%x)\n", cluster, *FAT_entry);
    return *FAT_entry;
}
static char formated_file_name[16];
char *FS_format_file_name(DirectoryEntry_t *entry) {
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
//        formated_file_name[j++] = '.';
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

FileInfo_t FS_read_one_file_info(DirectoryEntry_t *dir_entry, uint32_t cluster, FS_Info_t fs_info)
{
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
        ret.ParentStartCluster = cluster;
        ret.CurrentByte = 0;
        ret.StartCluster = ret.CurrentCluster = (((uint32_t)dir_entry->ClusterLow) << 16) | dir_entry->FirstCluster;
        ret.flags = 0;
        ret.mode = ENRTRY_MODE_WRITE;
        ret.pos = 0;
        ret.FileSize = dir_entry->FileSize;
        if ((ret.attributes & ATTR_DIRECTORY) == 1)
        {
            ret.FileSize = SECTOR_SIZE*fs_info.SectorsPerCluster;
        }
        ret.CurrentClusterOffset = 0;
    }
    return ret;
}
//Search a file under a directory
FileInfo_t *FS_find_file(uint32_t cluster, const char *filename, FileInfo_t *fp, FS_Info_t fs_info, SectorBuffer_t *buffer,
                         int recursive)
{
    if(recursive<=0)
        return nullptr;
    uint32_t FAT_content = FS_get_FAT_entry(cluster, fs_info, buffer);
//    if(FAT_content != FAT_MASK_EOC)
//    {
//        debug_printf("FS_fine_file: fat != FAT_MASK_EOC\n");
//        bochs_break();
//    }
//    FS_read_sector(buffer->data, (uint32_t)((fs_info.DataStartSector+(4 * cluster) / 512)));
    FS_read_sector(buffer->data, (uint32_t)((fs_info.DataStartSector+ cluster - 2)));
    size_t offset = 0;
    char LongFileNameBuffer[LONG_FILENAME_MAX_LEN];
    LongFileNameBuffer[0] = 0;
    for (; offset < fs_info.SectorsPerCluster*SECTOR_SIZE; offset += 0x20)
    {
        DirectoryEntry_t *CurrentOneFileInfo = (DirectoryEntry_t *)&buffer->data[offset];
        auto file_info=FS_read_one_file_info(CurrentOneFileInfo, cluster, fs_info);
        if ((file_info.attributes & 0x0f) == 0x0f)
        {
            //part of long filename
            strcpy(LongFileNameBuffer + strlen(LongFileNameBuffer), file_info.LongFilename);
        }
        else
        {
            strcpy(file_info.LongFilename,LongFileNameBuffer);
            //Got one file
            LongFileNameBuffer[0] = 0;
            if(file_info.filename[0] == '\000' || file_info.filename[0] == 0x7f)
            {
                //file is deleted
                continue;
            }
            else
            {
                if(strncmp(filename, file_info.filename, strlen(filename)) == 0
                        || strncmp(filename, file_info.LongFilename, strlen(filename)) == 0)
                {
                    *fp = file_info;
                    return fp;
                }
                else
                {
                    debug_printf("FS_find_file: %s is not %s\n", filename, file_info.filename);
                }
            }
        }
    }
//    if(FAT_content == FAT_MASK_EOC)
        return nullptr;
//    else
//        return FS_find_file(FAT_content, filename, fp, fs_info, buffer, recursive-1);
}
FileInfo_t * FS_fopen(const char *filename, const char *mode, FileInfo_t* fp, FS_Info_t fs_info, SectorBuffer_t* buffer)
{
    debug_printf("FS_fopen: %s\n", filename);
    if(fp == nullptr)
        return nullptr;
    uint32_t cluster = 0;
    size_t n = strlen(filename);
    if(n < 1 || filename[0] != '/')
        return nullptr;
    cluster = 2;
    fp->CurrentCluster = 2;
    fp->StartCluster = 2;
    fp->ParentStartCluster = 0xffffffff;
    fp->CurrentClusterOffset = 0;
    fp->CurrentSector = 0;
    fp->CurrentByte = 0;
    fp->attributes = ATTR_DIRECTORY;
    fp->pos = 0;
    fp->flags |= ENRTRY_FLAG_ROOT;
    fp->FileSize = 0xffffffff;
    fp->mode = ENRTRY_MODE_READ | ENRTRY_MODE_WRITE | ENRTRY_MODE_OVERWRITE;
    size_t i = 0, j = 0;
    while(true)
    {
        if(!filename[i] || filename[i] == '/')
        {
            if(i - j > 1)
            {
                char part_name[LONG_FILENAME_MAX_LEN];
                size_t len = i - j - 1;
                strncpy(part_name, filename + j + 1, len);
                part_name[len] = 0;
                fp = FS_find_file(cluster, part_name, fp, fs_info, buffer, 10);
                if(fp == nullptr)
                    break;
                cluster = fp->StartCluster;
            }
            j = i;
        }
        if(!filename[i])
            break;
        i++;
    }
    return fp;
}
//int FS_find_delete(uint32_t cluster, const char *filename, FileInfo_t * fp, FS_Info_t fs_info, SectorBuffer_t* buffer)
//{
//    uint32_t FAT_content = FS_get_FAT_entry(cluster, fs_info, buffer);
////    if(FAT_content != FAT_MASK_EOC)
////    {
////        debug_printf("FS_fine_file: fat != FAT_MASK_EOC\n");
////        bochs_break();
////    }
////    FS_read_sector(buffer->data, (uint32_t)((fs_info.DataStartSector+(4 * cluster) / 512)));
//    FS_read_sector(buffer->data, (uint32_t)((fs_info.DataStartSector+ cluster - 2)));
//    size_t offset = 0;
//    char LongFileNameBuffer[LONG_FILENAME_MAX_LEN];
//    LongFileNameBuffer[0] = 0;
//    for (; offset < fs_info.SectorsPerCluster*SECTOR_SIZE; offset += 0x20)
//    {
//        DirectoryEntry_t *CurrentOneFileInfo = (DirectoryEntry_t *)&buffer->data[offset];
//        auto file_info=FS_read_one_file_info(CurrentOneFileInfo, cluster, fs_info);
//        if ((file_info.attributes & 0x0f) == 0x0f)
//        {
//            //part of long filename
//            strcpy(LongFileNameBuffer + strlen(LongFileNameBuffer), file_info.LongFilename);
//        }
//        else
//        {
//            strcpy(file_info.LongFilename,LongFileNameBuffer);
//            //Got one file
//            LongFileNameBuffer[0] = 0;
//            if(file_info.filename[0] == '\000' || file_info.filename[0] == 0x7f)
//            {
//                //file is deleted
//                continue;
//            }
//            else
//            {
//                if(strncmp(filename, file_info.filename, strlen(filename)) == 0
//                   || strncmp(filename, file_info.LongFilename, strlen(filename)) == 0)
//                {
//                    buffer->data[offset] = 0xe5;
//                    FS_fclose(fp, buffer);
//                    return 0;
//                }
//            }
//        }
//    }
//    if(FAT_content == FAT_MASK_EOC)
//        return 1;
//    else
//        return FS_find_delete(FAT_content, filename, fp, fs_info, buffer);
//}
//int FS_fdelete(const char *filename, const char *mode, FS_Info_t fs_info, SectorBuffer_t* buffer)
//{
//    FileInfo_t f;
//    auto fp = &f;
//    uint32_t cluster = 0;
//    size_t n = strlen(filename);
//    if(n < 1 || filename[0] != '/')
//        return nullptr;
//    cluster = 2;
//    size_t i = 0, j = 0;
//    char part_name[LONG_FILENAME_MAX_LEN];
//    while(true)
//    {
//        if(!filename[i] || filename[i] == '/')
//        {
//            if(i - j > 1)
//            {
//                size_t len = i - j - 1;
//                strncpy(part_name, filename + j + 1, len);
//                part_name[len] = 0;
//                fp = FS_find_file(cluster, part_name, fp, fs_info, buffer);
//                if(fp == nullptr)
//                    continue;
//                cluster = fp->StartCluster;
//            }
//            j = i;
//        }
//        if(!filename[i])
//            break;
//        i++;
//    }
//    if(fp)
//        return FS_find_delete(fp->ParentStartCluster, part_name, fp, fs_info, buffer);
//    return 1;
//}

uint32_t FS_Find_Free_Cluster(uint32_t base, FS_Info_t fs_info, SectorBuffer_t *buffer)
{
    uint32_t cluster = base;
    uint32_t TotalCluster = fs_info.TotalSectors / fs_info.SectorsPerCluster;
    for (; cluster <= TotalCluster; cluster++)
    {
        if ((FS_get_FAT_entry(cluster, fs_info, buffer) & 0x0fffffff) == 0)
            return cluster;
    }
    cluster = 0;
    for (; cluster < base; cluster++) {
        if ((FS_get_FAT_entry(cluster, fs_info, buffer) & 0x0fffffff) == 0)
            return cluster;
    }
    return ERROR_VOLUME_FULL;
}
int FS_set_FAT_entry(uint32_t cluster, uint32_t value, FS_Info_t fs_info, SectorBuffer_t* buffer)
{
    uint32_t ErrorLevel = 0;
    uint32_t offset = 4 * cluster;
    uint32_t sector = fs_info.ReservedSectors + (offset / 512);
    ErrorLevel |= FS_fetch(sector, buffer);
    uint32_t *FAT_entry = ((uint32_t *) &(buffer->data[offset % 512]));
    DBG_PRINTF("FS_set_FAT_entry: Got FAT entry of cluster %d(%x)\n", cluster, *FAT_entry);
    if (*FAT_entry != value)
    {
        buffer->SectorFlags &= CURRENT_FLAG_DIRTY;
        *FAT_entry = value;
        DBG_PRINTF("FS_set_FAT_entry: Wrote FAT entry of cluster %d(%x)\n", cluster, value);
    }
    return ErrorLevel;
}
int FS_fseek(FileInfo_t *fp, int32_t base, long offset, FS_Info_t fs_info, SectorBuffer_t* buffer)
{
    long pos = base + offset;
    uint32_t cluster_offset;
    uint32_t temp;
    if ((pos > (long)fp->FileSize) && (fp->attributes!= ATTR_DIRECTORY))
    {
        return ERROR_SEEK_FILE;
    }
    else if ((long)fp->FileSize == pos)
    {
        fp->flags |= ENRTRY_FLAG_SIZECHANGED;
        fp->FileSize += 1;
    }
    else
    {
        cluster_offset = pos / (fs_info.SectorsPerCluster * 512);
        if (cluster_offset != fp->CurrentClusterOffset)
        {
            temp = cluster_offset;
            if (cluster_offset > fp->CurrentClusterOffset)
            {
                cluster_offset -= fp->CurrentClusterOffset;
            }
            else
            {
                fp->CurrentCluster = fp->StartCluster;
            }
            fp->CurrentClusterOffset = temp;
            while (cluster_offset > 0)
            {
                temp = FS_get_FAT_entry(fp->CurrentCluster, fs_info, buffer);
                if ((temp & FAT_MASK_EOC) != FAT_MASK_EOC)
                {
                    fp->CurrentCluster = temp;
                }
                else
                {
                    temp = FS_Find_Free_Cluster(fp->CurrentCluster, fs_info, buffer);
                    FS_set_FAT_entry(fp->CurrentCluster, temp, fs_info, buffer);
                    FS_set_FAT_entry(temp, FAT_MASK_EOC, fs_info, buffer);
                    fp->CurrentCluster = temp;
                }
                cluster_offset--;
                if (fp->CurrentCluster >= FAT_MASK_EOC)
                {
                    if (cluster_offset > 0)
                    {
                        return ERROR_SEEK_FILE;
                    }
                }
            }
        }
        fp->CurrentByte = (pos % (fs_info.SectorsPerCluster * 512));
        fp->pos = pos;
    }

    return 0;
}
uint32_t FS_cluster2sector(uint32_t cluster, FS_Info_t fs_info)
{
    uint32_t base = fs_info.DataStartSector;
    uint32_t offset = (cluster - 2)*fs_info.SectorsPerCluster;
    return base + offset ;
}
int FS_fread(uint8_t *dest, size_t size, FileInfo_t *fp, FS_Info_t fs_info, SectorBuffer_t *buffer) {
    uint32_t sector;
    for (; size > 0; size--)
    {
        sector = (fp->CurrentByte / 512)+ FS_cluster2sector(fp->CurrentCluster, fs_info) ;
        FS_fetch(sector, buffer);
        *dest++ = buffer->data[fp->CurrentByte % 512];
        if (fp->attributes & ATTR_DIRECTORY)
        {
            if (FS_fseek(fp, 0, fp->pos + 1, fs_info, buffer))
            {
                return ERROR_SEEK_FILE;
            }
        }
        else
        {
            if (FS_fseek(fp, 0, fp->pos + 1, fs_info, buffer))
            {
                return ERROR_SEEK_FILE;
            }
        }
    }
    return 0;
}
int FS_fwrite(uint8_t *src, uint32_t size, uint32_t count, FileInfo_t *fp, FS_Info_t fs_info, SectorBuffer_t* buffer) {
    uint32_t i, tracking, segsize;
    fp->flags |= ENRTRY_FLAG_DIRTY;
    while (count > 0) {
        i = size;
        fp->flags |= ENRTRY_FLAG_SIZECHANGED;
        while (i > 0) {
            uint32_t dest_sector = FS_cluster2sector(fp->CurrentCluster, fs_info) + (fp->CurrentByte / 512);
            FS_fetch(dest_sector, buffer);
            tracking = fp->CurrentByte % 512;
            segsize = (i<512 ? i : 512);
            memcpy(&buffer->data[tracking], src, segsize);
            buffer->SectorFlags |= ENRTRY_FLAG_DIRTY;
            if (fp->pos + segsize > fp->FileSize)
            {
                fp->FileSize += segsize - (fp->pos % 512);
            }
            if (FS_fseek(fp, 0, fp->pos + segsize, fs_info, buffer)) {
                return -1;
            }
            i -= segsize;
            src += segsize;
        }
        count--;
    }
    return size - i;
}
int FS_ffsync(FileInfo_t *fp, SectorBuffer_t *buffer)
{
    if (buffer->SectorFlags & CURRENT_FLAG_DIRTY)
        FS_fsync(buffer);
//    if ((fp->flags & ENRTRY_FLAG_SIZECHANGED) && !(fp->attributes & 0x10))
//    {
//        debug_printf("FS_ffsync: file %s SIZECHANGED\n", fp->filename);
//        FS_read_sector(buffer->data, (uint32_t)((fs_global_info.DataStartSector+ fp->ParentStartCluster - 2)));
//        buffer->SectorNumber = fp->ParentStartCluster;
//        size_t offset = 0;
//        for (; offset < fs_global_info.SectorsPerCluster*SECTOR_SIZE; offset += 0x20)
//        {
//            DirectoryEntry_t *CurrentOneFileInfo = (DirectoryEntry_t *)&buffer->data[offset];
//            auto file_info=FS_read_one_file_info(CurrentOneFileInfo, fp->ParentStartCluster, fs_global_info);
//            if ((file_info.attributes & 0x0f) == 0x0f)
//            {
//                continue;
//            }
//            else
//            {
//                if(file_info.filename[0] == '\000' || file_info.filename[0] == 0x7f)
//                {
//                    continue;
//                }
//                else
//                {
//                    if(strncmp(fp->filename, file_info.filename, strlen(fp->filename)) == 0
//                       || strncmp(fp->filename, file_info.LongFilename, strlen(fp->filename)) == 0)
//                    {
//                        debug_printf("FS_ffsync: %s: new file size %u\n", fp->FileSize);
//                        CurrentOneFileInfo->FileSize = fp->FileSize;
//                        buffer->SectorFlags |= CURRENT_FLAG_DIRTY;
//                        FS_fsync(buffer);
//                        buffer->SectorFlags &= ~CURRENT_FLAG_DIRTY;
//                    }
//                    else
//                    {
//                        debug_printf("FS_ffsync: %s is not %s\n", fp->filename, file_info.filename);
//                    }
//                }
//            }
//        }
//    }
    fp->flags &= ~ENRTRY_FLAG_DIRTY;
    fp->flags &= ~ENRTRY_FLAG_SIZECHANGED;
    return 0;
}

int FS_fclose(FileInfo_t *fp, SectorBuffer_t *buffer)
{
    FS_ffsync(fp,buffer);
    return 0;
}
int FS_touch(const char *filename, FS_Info_t fs_info, SectorBuffer_t* buffer) {
    DirectoryEntry_t entry;
    uint32_t cluster;
    const char *name;
    const char *ext;
    name = strrchr(filename, '/');
    if(name == nullptr)
    {
        return -1;
    }
    name = name + 1;
    ext = strrchr(filename, '.');
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
    char parent[strlen(filename)];
    strcpy(parent, filename);
    parent[name-filename] = 0;
    DBG_PRINTF("FS_touch: File parent: '%s'\n", parent);
    FileInfo_t f;
    FileInfo_t *fp = FS_fopen(parent, "r+", &f, fs_info, buffer);
    DBG_PRINTF("FS_touch: Creating new file: '%s'", filename);
    if(!fp) return 1;
    // Now we have the directory in which we want to create the file, open for overwrite
    do {
        //"seek" to the end
        FS_fread((uint8_t*)&entry, sizeof(DirectoryEntry_t), fp, fs_info, buffer);
        DBG_PRINTF("FS_touch: Skipping existing directory entry... %d\r\n", fp->pos);
    } while(entry.Filename[0] != '\x00');
    // Back up one entry, this is where we put the new filename entry
    FS_fseek(fp, -sizeof(DirectoryEntry_t), fp->pos, fs_info, buffer);
    cluster = FS_Find_Free_Cluster(0, fs_info, buffer);
    FS_set_FAT_entry(cluster, FAT_MASK_EOC, fs_info, buffer); // Marks the new cluster as the last one (but no longer free)
    // TODO shorten these entries with memset
    entry.Attributes = ATTR_ARCHIVE;
    entry.Reserved = 0;
    entry.CreationTimeMs = 0x6F;
    entry.CreationTime = 0xAE70;
    entry.CreationDate = 0x4CD5;
    entry.LastAccessTime = 0x4CD5;
    entry.ClusterLow = (cluster>>16) & 0xffff;
    entry.ModifiedTime = 0xAE71;
    entry.ModifiedDate = 0x4CD5;
    entry.FirstCluster = cluster & 0xffff;
    entry.FileSize = 0;
    strncpy((char*)entry.Filename, name, name_len < 8 ? name_len : 8);
    strncpy((char*)entry.Extension, ext ? ext : "   ", 3);
//    strncpy((char*)entry.Extension, ext, 3);
    for(int i = 0; i < 8; ++i)
    {
        if(entry.Filename[i] == 0)
            entry.Filename[i] = ' ';
    }
    for(int i = 0; i < 3; ++i)
    {
        if(entry.Extension[i] == 0)
            entry.Extension[i] = ' ';
    }
    printf("FS_create: Filename: %s\n", entry.Filename);
    printf("FS_create: Extension: %s\n", entry.Extension);
    FS_fwrite((uint8_t*)&entry, sizeof(DirectoryEntry_t), 1, fp, fs_info, buffer);
    memset(&entry, 0, sizeof(DirectoryEntry_t));
    entry.Filename[0] = '\x00';
    FS_fwrite((uint8_t*)&entry, sizeof(DirectoryEntry_t), 1, fp, fs_info, buffer);
    FS_fclose(fp, buffer);
    return 0;
}

void Show_FS_Info(FS_Info_t fs_info)
{
    printf("-----------------------------FS INFO-------------------------------\n");
    printf("This Volume Total have %u Sectors:\n", fs_info.TotalSectors);
    printf("Cluster Size:%d\n", fs_info.SectorsPerCluster*SECTOR_SIZE);
    for (int i = 0; i < 11; i++)
    {
        printf("%c", fs_info.Label[i]);
    }
    printf("\n");
    printf("Reserved Sectors: %u\n", fs_info.ReservedSectors);
    printf("Fat zone Total have %u Sectors\n:", fs_info.FatSize);
    printf("Data zone Start from Cluster: %u\n", fs_info.DataStartSector);
    printf("Data zone Total have %u Sectors:\n", fs_info.DataSectors);
    printf("Data zone Total have %u Clusters:\n", fs_info.DataClusters);
    printf("-----------------------------FS INFO END---------------------------\n");
}

void Read_Print_File(const char *filename, FileInfo_t* fp,FS_Info_t fs_info, SectorBuffer_t* buffer)
{
    printf("-------------------Open Normal File----------------------------\n");
    fp = FS_fopen(filename, "r", fp, fs_info, buffer);
    if(fp == nullptr)
    {
        printf("File Not found\n");
        return;
    }
    printf("File %s opened\n", filename);
    printf("Parent Directory at Cluster: %u\n", fp->ParentStartCluster);
    printf("File Start at Cluster: %u\n", fp->StartCluster);
//    printf("File Start at Cluster: %u\n", fp->CurrentSector);
    printf("File size (byte): %u\n", fp->FileSize);
    char dest[32];
    memset(dest, 0, 32);
    FS_fread((uint8_t*)&dest[0], 32, fp, fs_info, buffer);
    dest[31] = 0;
    printf("-------------------File Content---------------------\n");
    printf("%s\n", dest);
    printf("-------------------File Content END------------------\n");
    printf("-------------------Open Normal File END------------------------\n");
}