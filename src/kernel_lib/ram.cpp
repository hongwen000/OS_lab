//
// Created by 李新锐 on 20/05/2018.
//
#include "ram.h"
#include "../libc/stdio.h"
#include "debug_printf.h"
#include "page.h"

extern HHOS_info_t HHOS_info;
extern uint8_t kernstart;
extern uint8_t code;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t kernend;
static uint32_t pm_node[PMM_MAX_NODES + 1];
static uint32_t pm_node_cnt = 0;
static uint32_t pm_node_size = 0;
void ram_init()
{
    int* pMCR_count = (int*)0x400;
    ADRS* pADRS = (ADRS*)0x500;
    for(int i = 0; i < *pMCR_count; ++i)
    {
//        printf("Mem Region %u: Base: 0x%x%x, Len: 0x%x%x, Type: %s\n", i, pADRS->BaseAddrHigh, pADRS->BaseAddrLow,
//               pADRS->LengthHigh, pADRS->LengthLow, memory_info[pADRS->Type-1]);
        if(pADRS->BaseAddrLow == 0x100000)
        {
            HHOS_info.phy_mem = (pADRS->LengthHigh) << sizeof(uint32_t) | (pADRS->LengthLow);
            uint32_t addr = ((uint32_t)&kernend + PMM_PAGE_SIZE) & 0xfffff000;
            uint32_t limit = pADRS->BaseAddrLow + HHOS_info.phy_mem;
            while (addr < limit && addr <= PMM_MAX_RAM)
            {
                pm_node[pm_node_cnt] = addr;
                pm_node_cnt++;
                addr += PMM_PAGE_SIZE;
            }
            pm_node_size = pm_node_cnt;
        }
        ++pADRS;
    }
    printf("-----------------------------MEM INFO------------------------------\n");
    printf("0x%x bytes memory Installed\n", HHOS_info.phy_mem);
    printf("Kernel start at: 0x%x, end at0x%x, text_size %ukb\n", &kernstart, &kernend, (&kernend - &kernstart) / 1024);
    printf("%d pages available for allocation (%d kb)\n", pm_node_cnt, pm_node_cnt * 4);
    printf("-----------------------------MEM INFO END--------------------------\n");
}

uint32_t ram_alloc()
{
    if(pm_node_size == 0)
    {
        debug_printf("No memory\n");
        bochs_break();
        return 0;
    }
    debug_printf("ram_alloc: give you physical memory 0x%x\n", pm_node[pm_node_size - 1]);
    memset((char*)pm_node[pm_node_size - 1], 0, PAGE_SIZE);
    return pm_node[--pm_node_size];
}

void ram_free(uint32_t addr)
{
    if(addr & 0xfff)
    {
        debug_printf("Can not free 0x%x, which is not 4k-aligned\n", addr);
        bochs_break();
        return;
    }
    pm_node[pm_node_size++] = addr;
    if(pm_node_size + 1 > pm_node_cnt)
    {
        debug_printf("All memory is free!");
        bochs_break();
        return;
    }
}
