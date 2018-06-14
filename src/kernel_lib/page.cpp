//
// Created by 李新锐 on 20/05/2018.
//
#include "page.h"
#include "debug_printf.h"
#include "ram.h"

extern HHOS_info_t HHOS_info;

//页目录
pde_t pgd_kern[PDE_SIZE] __attribute__((aligned(PAGE_SIZE)));
//页表项
static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute__((aligned(PAGE_SIZE)));
//加载页表
inline void vmm_switch_pgd(uint32_t pde){
    __asm__ volatile ("mov %0, %%cr3": :"r"(pde));
}
//刷新页表缓存
static inline void vmm_reflush(uint32_t va){
    __asm__ volatile ("invlpg (%0)"::"a"(va));
}
//开启分页
static inline void vmm_enable(){
    uint32_t cr0;

    __asm__ volatile ("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= CRO_PG;
    __asm__ volatile ("mov %0, %%cr0" : : "r" (cr0));
}

void vmm_init(){
    uint32_t i, j;

    // map 4G memory, physcial address = virtual address
    for (i = 0, j = 0; i < PTE_COUNT; i++, j++){
        pgd_kern[i] = (uint32_t)pte_kern[j] | PTE_P | PTE_R | PTE_K;
    }

    uint32_t *pte = (uint32_t *)pte_kern;
    for (i = 1; i < PTE_COUNT*PTE_SIZE; i++){
        pte[i] = (i << 12) | PTE_P | PTE_P | PTE_K;
    }

    vmm_switch_pgd((uint32_t)pgd_kern);

    vmm_enable();
}

//将内存进行映射
//页表目录/虚拟地址/物理地址/flags
void vmm_map(pde_t *pgdir, uint32_t va, uint32_t pa, uint32_t flags){
    if(va >= USER_BASE)
        debug_printf("vmm_map: 1. Try to map ram %x to %x\n", pa, va);
    uint32_t pde_idx = PDE_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);

    pte_t *pte = (pte_t *)(pgdir[pde_idx] & PAGE_MASK);
    if(va >= USER_BASE)
        debug_printf("vmm_map: 2. pte of USER va 0x%x is at 0x%x\n", va, pte);

    if (!pte){
        if (va >= USER_BASE){
            pte = (pte_t *)ram_alloc();
            debug_printf("vmm_map: 2.5it is used for storing page entry\n");
            pgdir[pde_idx] = (uint32_t)pte | PTE_P | flags;
        } else {
            pte = (pte_t *)(pgd_kern[pde_idx] & PAGE_MASK);
            pgdir[pde_idx] = (uint32_t)pte | PTE_P | flags;
            return;
        }
    }

    pte[pte_idx] = (pa & PAGE_MASK) | PTE_P | flags;
    if(va >= USER_BASE)
        debug_printf("vmm_map: 3.wrote page table: pte = 0x%x, pte_idx = %u, &pte[pte_idx] = 0x%x, pte[pte_idx] = 0x%x\n", pte, pte_idx, &(pte[pte_idx]), pte[pte_idx]);

}
int vmm_get_mapping(pde_t *pgdir, uint32_t va, uint32_t *pa){
    uint32_t pde_idx = PDE_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);
    debug_printf("vmm_get_mapping: va 0x%x, pde_idx: %u, pte_idx: %u\n", va, pde_idx, pte_idx);

    pte_t *pte = (pte_t *)(pgdir[pde_idx] & PAGE_MASK);
    debug_printf("vmm_get_mapping: pte* = 0x%x\n", pte);
    if (!pte){
        debug_printf("vmm_get_mapping: virtual address 0x%x is unmapped\n", va);
        return 0;
    }
    if (pte[pte_idx] != 0 && (pte[pte_idx] & PTE_P)){
        if(pa)
            *pa = pte[pte_idx] & PAGE_MASK;
        debug_printf("get_mapping: pgdir 0x%x virtual address 0x%x is mapped to 0x%x\n", pgdir, va, pte[pte_idx] & PAGE_MASK);
        return 1;
    }
    return 0;
}

void kvm_init(pde_t *pgdir){
    uint32_t addr;
    // uint32_t limit = PAGE_ALIGN_UP((uint32_t)&kernend);

    debug_printf("kvm_init: pgdir: 0x%x\n", pgdir);

    if(pgdir == 0)
        debug_puts("kvm_init: null pgdir\n");

    for (addr = 0; addr < HHOS_info.phy_mem; addr += PAGE_SIZE){
        vmm_map(pgdir, addr, addr, PTE_P | PTE_R | PTE_K);
    }
}
/* build a map of user space for a initproc's page table */
void uvm_init_fst(pde_t *pgdir, char *init, uint32_t size){
    char *user_tack;
    debug_puts("uvm_init_fst: alloc memory for user stack of init\n");
    user_tack = (char *)ram_alloc();
    memset(user_tack, 0, PAGE_SIZE);
    vmm_map(pgdir, USER_BASE, (uint32_t) user_tack, PTE_U | PTE_P | PTE_R);

    char *room;

    if(size < PAGE_SIZE)
        debug_puts("uvm_init_fst: size < PAGE_SIZE\n");

    debug_puts("uvm_init_fst: alloc memory for init\n");
    room = (char *)ram_alloc();

    memset(room, 0, PAGE_SIZE);
    debug_printf("init = 0x%x size = %d\n", init, size);
    memcpy(room, init, size);
    debug_printf("uvm_init_fst: clear and copy\n");

    vmm_map(pgdir, USER_TEXT_BASE, (uint32_t)room, PTE_U | PTE_P | PTE_R);
    debug_printf("uvm_init_fst: map\n");
}
void uvm_switch(PCB *pp){
    tss_set(SEL_KDATA << 3, (uint32_t)pp->kern_stack + PAGE_SIZE);
    vmm_switch_pgd((uint32_t)pp->pgdir);
}
pde_t *uvm_copy(pte_t *pgdir, uint32_t text_size){
    pde_t *pgd;
    uint32_t i, pa, mem;

    pgd = (pde_t *)ram_alloc();
    memset(pgd, 0, PAGE_SIZE);

    kvm_init(pgd);

    debug_printf("uvm_copy: copy pgdir 0x%x -> 0x%x, size: %d\n",pgdir, pgd, text_size);

    for (i = 0; i < USER_STACK_SIZE + text_size; i += PAGE_SIZE){
        if(!vmm_get_mapping(pgdir, USER_BASE + i, &pa))
            debug_puts("uvm_copy: pte not exixt or no present\n");

        mem = ram_alloc();
        memcpy((void *)mem, (void *)pa, PAGE_SIZE);

        debug_printf("uvm_copy: phyaddr: 0x%x -> 0x%x\n", pa, mem);

        vmm_map(pgd, USER_BASE + i, mem, PTE_R | PTE_U | PTE_P); // TODO (?)
    }
    return pgd;
}
pde_t *uvm_copy_thread(pte_t *pgdir, uint32_t text_size){
    pde_t *pgd;
    uint32_t i, pa, mem;

    pgd = (pde_t *)ram_alloc();
    memset(pgd, 0, PAGE_SIZE);

    kvm_init(pgd);

    debug_printf("uvm_copy thread: copy pgdir 0x%x -> 0x%x, size: %d\n",pgdir, pgd, text_size);

    for (i = 0; i < USER_STACK_SIZE; i += PAGE_SIZE){
        if(!vmm_get_mapping(pgdir, USER_BASE + i, &pa))
            debug_puts("uvm_copy_thread: pte not exixt or no present\n");

        mem = ram_alloc();
        memcpy((void *)mem, (void *)pa, PAGE_SIZE);

        debug_printf("uvm_copy_thread: Alloc new user stack, phyaddr: 0x%x -> 0x%x\n", pa, mem);

        vmm_map(pgd, USER_BASE + i, mem, PTE_R | PTE_U | PTE_P); // TODO (?)
    }
    for (i = USER_STACK_SIZE; i < USER_STACK_SIZE + text_size; i += PAGE_SIZE)
    {
        if(!vmm_get_mapping(pgdir, USER_BASE + i, &pa))
            debug_puts("uvm_copy_thread: pte not exixt or no present\n");

        vmm_map(pgd, USER_BASE + i, pa, PTE_R | PTE_U | PTE_P); // TODO (?)
    }
    return pgd;
}
void uvm_free(pte_t *pgdir){
    uint32_t i;

    if(!pgdir)
        debug_puts("uvm_free: no page table\n");

    i = PDE_INDEX(USER_BASE);

    for (; i < PTE_COUNT; i++){
        if (pgdir[i] & PTE_P){
            debug_printf("uvm_free: free pte 0x%x\n", pgdir[i]);
            ram_free(pgdir[i] & PAGE_MASK);
        }
    }

    ram_free((uint32_t)pgdir);
}
void vmm_test()
{
    sys_read_hard_disk(SEL_KERN_DATA, (uint32_t)0x450000, 64, 1);
    vmm_map(pgd_kern, 0x400000, 0x450000,  PTE_P | PTE_R | PTE_K);
    const char* help = (const char*)0x400000;
    debug_puts(help);
//    hhos_assert(vmm_get_mapping(pgd_kern, (uint32_t)help, nullptr));
}
int uvm_alloc(pte_t *pgdir, uint32_t old_sz, uint32_t new_sz){
    uint32_t mem;
    uint32_t start;

    debug_printf("uvm_alloc: pgdir: 0x%x, old_size 0x%x -> new_size 0x%x\n", pgdir, old_sz, new_sz);

    if (new_sz < old_sz){
        return old_sz;
    }

    for (start = PAGE_ALIGN_UP(old_sz); start < new_sz; start += PAGE_SIZE){
        mem = ram_alloc();
        debug_printf("uvm_alloc: that's for storing really elf image\n", pgdir, start, mem);
        memset((void *)mem, 0, PAGE_SIZE);
        debug_printf("uvm_alloc: call vmm_map(pgdir = 0x%x, start = 0x%x, mem = 0x%x)\n", pgdir, start, mem);
        vmm_map(pgdir, start, mem, PTE_P | PTE_R | PTE_U);  // diff with PTE_U PTE_R ?
    }

    return new_sz;
}
int uvm_load(pte_t *pgdir, uint32_t addr, char* ip, uint32_t off, uint32_t size){
    uint32_t i, n, pa;

    debug_printf("uvm_load: pgdir: 0x%x addr: 0x%x ip:0x%x offset: 0x%x size: 0x%x\n", pgdir, addr, ip, off, size);

    if((uint32_t)addr % PAGE_SIZE != 0)
        debug_puts("uvm_load: addr must page aligned\n");

    for (i = 0; i < size; i += PAGE_SIZE){
        if(vmm_get_mapping(pgdir, addr + i, &pa) == 0)
            debug_puts("uvm_load: address no mapped\n");
        if (size - i < PAGE_SIZE){
            n = size - i;
        } else {
            n = PAGE_SIZE;
        }

        // pa = va now
        memcpy((char*)pa, ip + off + i, n);
    }
    return 0;
}