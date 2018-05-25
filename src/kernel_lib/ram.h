//
// Created by 李新锐 on 20/05/2018.
//

#ifndef TEST_RAM_H
#define TEST_RAM_H

#include "../include/defines.h"
#define ADDR_RANGE_MEMORY   1
#define ADDR_RANGE_RESERVED 2
#define ADDR_RANGE_UNDEFINE 3

#define PMM_MAX_RAM 0x20000000 // 512Mb
#define PMM_PAGE_SIZE 0x1000
#define PMM_MAX_NODES (PMM_MAX_RAM/PMM_PAGE_SIZE)
#define PMM_PAGW_MASK 0xfffff000

struct pm_node_t{
    bool free;
    uint32_t pm_addr;
    pm_node_t* next;
};
struct ADRS
{
    uint32_t BaseAddrLow;
    uint32_t BaseAddrHigh;
    uint32_t LengthLow;
    uint32_t LengthHigh;
    uint32_t Type;
} __attribute__((packed));

static char *memory_info[] =
        {
                "Usable RAM",
                "Reserved",
                "ACPI reclaimable",
                "ACPI NVS",
                "Area containing bad memory"
        };
void ram_init();
uint32_t ram_alloc();
void ram_free(uint32_t addr);
#endif //TEST_RAM_H
