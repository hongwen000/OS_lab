/* gdt.c 
 * This file is modified form Bram's Kernel Development Tutorial
 * set the new gdt, the new gdt table has 256 entrys
 */
// std
#include "../include/defines.h"
// x86
#include "pm.h"
// libs
#include "../libc/string.h"

struct tss_entry tss;

static struct gdt_entry gdt[NGDT];  // we hava 256 gdt entry
struct gdt_ptr gp;

extern "C" void gdt_flush();    // extern func in loader.asm

void tss_install(){
    __asm__ volatile("ltr %%ax" : : "a"((SEL_TSS << 3)));
}

void gdt_install(uint8_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags){

    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xffff);
    gdt[num].base_middle = (base >> 16) & 0xff;
    gdt[num].base_high = (base >> 24) & 0xff;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xffff);
    gdt[num].limit_high = ((limit >> 16) & 0x0f);

    /* Finally, set up the granularity and access flags */
    gdt[num].flags = flags;

    access |= AC_RE;
    gdt[num].access = access;
}


void gdt_init(){
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * NGDT) - 1; //gdtr limit <- text_size of gdt table - 1
    gp.base = (uint32_t)&gdt;

    /* null descriptor */
    gdt_install(0, 0, 0, 0, 0);  
    /* kernel codenfo segment type: code addr: 0 limit: 4G gran: 4KB sz: 32bit */
    gdt_install(SEL_KCODE, 0, 0xfffff, AC_RW|AC_EX|AC_DPL_KERN|AC_PR, GDT_GR|GDT_SZ);
    /* kernel data segment type: data addr: 0 limit: 4G gran: 4KB sz: bit 32bit */
    gdt_install(SEL_KDATA, 0, 0xfffff, AC_RW|AC_DPL_KERN|AC_PR, GDT_GR|GDT_SZ);
    /* kernel video segment type: data addr: 0xB8000 limit: 1Mb gran: 1byte sz: 32bit */
    gdt_install(SEL_VIDEO, 0xB8000, 0xfffff, AC_RW|AC_DPL_KERN|AC_PR, GDT_SZ);
    /* kernel stack segment type: data addr: 0x6C00-0x7C00 direction: down limit: 4Kb gran: 4Kb sz: 32bit */
    gdt_install(SEL_STACK, 0, 0xfffff, AC_RW|AC_DPL_KERN|AC_PR, GDT_GR|GDT_SZ);
    /* user code segment type: code addr: 0 limit: 4G gran: 4KB sz: 32bit */
    gdt_install(SEL_UCODE, 0, 0xfffff, AC_RW|AC_EX|AC_DPL_USER|AC_PR, GDT_GR|GDT_SZ); 
    /* user code segment type: data addr: 0 limit: 4G gran: 4KB sz: 32bit */
    gdt_install(SEL_UDATA, 0, 0xfffff, AC_RW|AC_DPL_USER|AC_PR, GDT_GR|GDT_SZ);
    gdt_install(SEL_TSS, (uint32_t)&tss, sizeof(tss),AC_PR|AC_AC|AC_EX, GDT_GR); 
    /* for tss, access_reverse bit is 1 */
    gdt[SEL_TSS].access &= ~AC_RE;
    /* user code segment type: code addr: 0 limit: 4G gran: 4KB sz: 32bit */
    gdt_install(SEL_UCODE0, 0x200000, 0x1ffff, AC_RW|AC_EX|AC_DPL_KERN|AC_PR, GDT_GR|GDT_SZ);
    /* user code segment type: data addr: 0 limit: 4G gran: 4KB sz: 32bit */
    gdt_install(SEL_UDATA0, 0x200000, 0x1ffff, AC_RW|AC_DPL_KERN|AC_PR, GDT_GR|GDT_SZ);
    /* user code segment type: code addr: 0 limit: 4G gran: 4KB sz: 32bit */
    gdt_install(SEL_UCODE1, 0x300000, 0x1ffff, AC_RW|AC_EX|AC_DPL_KERN|AC_PR, GDT_GR|GDT_SZ);
    /* user code segment type: data addr: 0 limit: 4G gran: 4KB sz: 32bit */
    gdt_install(SEL_UDATA1, 0x300000, 0x1ffff, AC_RW|AC_DPL_KERN|AC_PR, GDT_GR|GDT_SZ);
    /* user code segment type: code addr: 0 limit: 4G gran: 4KB sz: 32bit */
    gdt_install(SEL_UCODE2, 0x400000, 0x1ffff, AC_RW|AC_EX|AC_DPL_KERN|AC_PR, GDT_GR|GDT_SZ);
    /* user code segment type: data addr: 0 limit: 4G gran: 4KB sz: 32bit */
    gdt_install(SEL_UDATA2, 0x400000, 0x1ffff, AC_RW|AC_DPL_KERN|AC_PR, GDT_GR|GDT_SZ);
    /* user code segment type: code addr: 0 limit: 4G gran: 4KB sz: 32bit */
    gdt_install(SEL_UCODE3, 0x500000, 0x1ffff, AC_RW|AC_EX|AC_DPL_KERN|AC_PR, GDT_GR|GDT_SZ);
    /* user code segment type: data addr: 0 limit: 4G gran: 4KB sz: 32bit */
    gdt_install(SEL_UDATA3, 0x500000, 0x1ffff, AC_RW|AC_DPL_KERN|AC_PR, GDT_GR|GDT_SZ);

    gdt_flush();
    tss_install();
}

void tss_set(uint16_t ss0, uint32_t esp0){
    memset((void *)&tss, 0, sizeof(tss));
    tss.ss0 = ss0;
    tss.esp0 = esp0;
    tss.iopb_off = sizeof(tss);
}
