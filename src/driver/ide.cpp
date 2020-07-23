//
// Created by 李新锐 on 21/04/2018.
//
#include "./ide.h"
#include "../include/defines.h"
#include "../kernel_lib/sys_utility.h"

#ifdef IDE_TEST

#include "../libc/stdio.h"
#include "../libc/assert.h"

#endif

static ide_request* ide_req_que = nullptr;
bool ide_disk_detected = false;


int ide_wait(int check)
{
    #ifdef IDE_TEST
    printf("ide_wait: check = %d\n", check);
    #endif
    int r;
    for(int i = 0; i < 4; ++i)
    {
        while ((r = sys_inb(IDE_PORT_STATUS)) & IDE_BSY)
        {
        }
    }
    if (check && (r & (IDE_DF | IDE_ERR)))
    {
        #ifdef IDE_TEST
        printf("ide_wait: error\n");
        #endif
        return -1;
    }
    return 0;
}

void ide_init()
{
    ide_wait(0);
    sys_outb(IDE_PORT_CURRENT, 0xe0 | (1<<4));
    for(int i = 0; i < 1000; ++i)
    {
        if(sys_inb(IDE_PORT_STATUS) != 0)
        {
            ide_disk_detected = 1;
            break;
        }
    }
    sys_outb(IDE_PORT_CURRENT, 0xe0 | (0<<4));
}
void ide_deal_req(struct ide_request* b)
{
    #ifdef IDE_TEST
    printf("ide_deal_req: b->lba=%d\n", b->lba);
    #endif
    ide_wait(false);
    uint32_t block_per_buffer = (IDE_BUF_SIZE / IDE_BLOCK_SIZE);
    uint32_t ide_block_n = b->lba * block_per_buffer;
    //Base address of primary channel control port
    sys_outb(IDE_PORT_ALTSTATUS, 0);
    sys_outb(IDE_PORT_SECT_COUNT, IDE_BUF_SIZE / IDE_BLOCK_SIZE);
    sys_outb(IDE_PORT_LBA0, ide_block_n & 0xff);
    sys_outb(IDE_PORT_LBA1, (ide_block_n >> 8) & 0xff);
    sys_outb(IDE_PORT_LBA2, (ide_block_n >> 16) & 0xff);
    sys_outb(IDE_PORT_CURRENT, 0xe0 | ((b->dev & 1) << 4) | ((ide_block_n >> 24) & 0x0f));
    if (b->cmd & B_DIRTY)
    {
#ifdef IDE_TEST
        printf("ide_start: write blk-%d\n", b->lba);
#endif
        sys_outb(IDE_PORT_CMD, block_per_buffer == 1 ? IDE_CMD_WRITE : IDE_CMD_WRITE_MULTIPLE);
        sys_outsl(IDE_PORT_DATA, b->buf, IDE_BUF_SIZE/4);
    } 
    else {
#ifdef IDE_TEST
        printf("ide_start: read blk-%d\n", b->lba);
#endif
        sys_outb(IDE_PORT_CMD, block_per_buffer == 1 ? IDE_CMD_READ : IDE_CMD_READ_MULTIPLE);
        ide_wait(true);
        sys_insl(IDE_PORT_DATA, b->buf, IDE_BUF_SIZE/4);
    }
}

void sys_ide_handler()
{
    sys_outb(IDE_PORT_ALTSTATUS, ATA_DEV_CONTROL_nIEN);
    struct ide_request *b = ide_req_que;
    if (b == nullptr)
        return;
    ide_req_que = b->next_req;
    if (!(b->cmd & B_DIRTY) && (ide_wait(true) == 0))
    {
        sys_insl(IDE_PORT_DATA, b->buf, IDE_BUF_SIZE/4);
    }
    b->cmd |= B_VALID;
    b->cmd &= ~B_DIRTY;
    wakeup(b);
#ifdef IDE_TEST
    printf("sys_ide_handler: blk-%d, flags: ", b->lba);
    if (b->cmd & B_BUSY) printf("B_BUSY ");
    if (b->cmd & B_DIRTY) printf("B_DIRTY ");
    if (b->cmd & B_VALID) printf("B_VALID ");
    printf("\n");
#endif
    if (ide_req_que)
    {
        ide_deal_req(ide_req_que);
    }
    sys_outb(IDE_PORT_ALTSTATUS, 0x0);

}

void ide_rw(ide_request* b)
{
#ifdef IDE_TEST
    hhos_assert(b->cmd & B_BUSY);
    hhos_assert((b->cmd & (B_VALID|B_DIRTY)) != B_VALID);
#endif
    ide_request** pp;
    b->next_req = nullptr;
#ifdef IDE_TEST
    printf("ide_rw: request queue: ");
#endif
    for(pp = &ide_req_que; *pp != nullptr; pp = &(*pp)->next_req)
    {
#ifdef IDE_TEST
        printf("%d -> ",(*pp)->lba);
#endif
    }
#ifdef IDE_TEST
    printf("%d ", b->lba);
    if (b->cmd & B_BUSY) printf("B_BUSY ");
    if (b->cmd & B_DIRTY) printf("B_DIRTY ");
    if (b->cmd & B_VALID) printf("B_VALID ");
    printf("\n");
#endif
    *pp = b;
    if (ide_req_que == b)
    {
        ide_deal_req(b);
    }
    // while ((b->cmd & (B_VALID|B_DIRTY)) != B_VALID)
    {
    //     sys_do_sleep(b);
    }
}

#ifdef IDE_TEST

void ide_print_blk(ide_request *b){
    printf("ide_print_blk: blk-%d, flags: ", b->lba);
    if (b->cmd & B_BUSY) printf("B_BUSY ");
    if (b->cmd & B_DIRTY) printf("B_DIRTY ");
    if (b->cmd & B_VALID) printf("B_VALID ");
    printf("\n");
    int i,j;
    for (i = 0; i < IDE_BUF_SIZE/16; i += 32){
        for (j = i; j < i + 32; j++){
            printf("%c",b->buf[j]);
        }
        printf("\n");
    }
}

ide_request buffer;  // used for test
void ide_test(){
    buffer.dev = 0;
    buffer.lba= 0;
    buffer.cmd = B_BUSY;
    ide_rw(&buffer);
    printf("Read hard disk over\n");
    ide_print_blk(&buffer);
//    int i = 0;
//    for (i = 0; i < IDE_BUF_SIZE; i++){
//        buffer.buf[i] = 'a';
//    }
//    buffer.cmd = B_BUSY|B_DIRTY;
//    ide_rw(&buffer);
//
//    buffer.cmd = B_BUSY;
//    ide_rw(&buffer);
//    ide_print_blk(&buffer);

}

#endif