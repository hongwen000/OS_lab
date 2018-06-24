//
// Created by 李新锐 on 22/05/2018.
//
#include "../include/defines.h"
#include "exec.h"
#include "../kernel_lib/debug_printf.h"
#include "../kernel_lib/page.h"
#include "../kernel_lib/ram.h"
#include "elf.h"
#include "../fs/sys_uio.h"
#include "../libc/stdio.h"

void print_elfhdr(struct elf32hdr *eh){
    debug_printf("print_elfhdr:\n");
    debug_printf("    magic: 0x%x\n", eh->magic);
    debug_printf("    elf: %s\n", eh->elf);
    debug_printf("    entry: 0x%x\n", eh->entry);
    debug_printf("    phoff: 0x%x\n", eh->phoff);
    debug_printf("    phnum: %d\n", eh->phnum);
}

void print_proghdr(struct proghdr *ph){
    debug_printf("print_proghdr:\n");
    debug_printf("    type: 0x%x\n", ph->type);
    debug_printf("    off: 0x%x\n", ph->off);
    debug_printf("    vaddr: 0x%x\n", ph->vaddr);
    debug_printf("    paddr: 0x%x\n", ph->paddr);
    debug_printf("    filesz: 0x%x\n", ph->filesz);
    debug_printf("    memsz: 0x%x\n", ph->memsz);
    debug_printf("    flag: 0x%x\n", ph->flag);
    debug_printf("    align: 0x%x\n", ph->align);
}
static char binary_image_buf[128 * 512];
int sys_do_exec(uint32_t n){
    int i;
    char *s, *name;
    uint32_t sz, sp, off, pa, ustack[3 + MAX_ARGC + 1];
    pde_t *pgdir, *old_pgdir;
    struct elf32hdr eh;
    struct proghdr ph;

    debug_printf("exec: try to read form disk...\n");

    pgdir = 0;
    i = off = 0;

    pgdir = (pde_t *)ram_alloc();
    debug_printf("exec: that's addr of user prog PAGE DIR\n");
    kvm_init(pgdir);
    sys_read_hard_disk(SEL_KERN_DATA, (uint32_t)(char*)(binary_image_buf), n, 40);
//    uint32_t p = 0;
//    p += sizeof(elf32hdr);
    memcpy(&eh, binary_image_buf, sizeof(elf32hdr));
    debug_printf("exec: parsering elf\n");
    print_elfhdr(&eh);

    if (eh.magic != ELF_MAGIC){
        debug_printf("exec: bad\n");
        if (pgdir){
            uvm_free(pgdir);
        }
        return -1;
    }

    debug_printf("exec: load program section to memory\n");

    // load program to memory
    sz = USER_TEXT_BASE;
    for (i = 0, off = eh.phoff; i < eh.phnum; i++, off += sizeof(ph)){
        memcpy(&ph, binary_image_buf + off, sizeof(ph));
//        p += sizeof(ph);
        print_proghdr(&ph);
        if (ph.type != ELF_PROG_LOAD){
            continue;
        }
        if (ph.memsz < ph.filesz){
            debug_printf("exec: bad\n");
            if (pgdir){
                uvm_free(pgdir);
            }
            return -1;
        }
        if ((sz = uvm_alloc(pgdir, sz, ph.vaddr + ph.memsz)) == 0){
            debug_printf("exec: bad\n");
            if (pgdir){
                uvm_free(pgdir);
            }
            return -1;
        }
        if (uvm_load(pgdir, ph.vaddr, binary_image_buf, ph.off, ph.filesz) < 0){
            debug_printf("exec: bad\n");
            if (pgdir){
                uvm_free(pgdir);
            }
            return -1;
        }
//        p += ph.filesz;
    }


    auto top = PAGE_ALIGN_UP(sz);
    debug_printf("exec: build user stack\n");
    /* build user stack */
    sz = USER_BASE;
    if ((sz = uvm_alloc(pgdir, sz, sz + USER_STACK_SIZE)) == 0){
        debug_printf("exec: bad\n");
        if (pgdir){
            uvm_free(pgdir);
        }
        return -1;
    }
    sp = sz;
    if (vmm_get_mapping(pgdir, sz - USER_STACK_SIZE, &pa) == 0){  // sz is no mapped
        debug_printf("exec: bad\n");
        if (pgdir){
            uvm_free(pgdir);
        }
        return -1;
    }
    pa += USER_STACK_SIZE;
    uint32_t argc = 1;
    ustack[3+argc] = 0;
    ustack[0] = 0xffffffff;
    ustack[1] = argc;
    ustack[2] = sp - (argc+1)*4;  // argv pointer

    sp -= (3 + argc + 1)*4;
    pa -= (3 + argc + 1)*4;
    memcpy((void *)pa, ustack, (3 + argc + 1)*4);   // combine

    debug_printf("exec: prepare for new process `%s`\n", "");

    asm volatile("cli");
    strcpy(current_proc->name, "");

    old_pgdir = current_proc->pgdir;
    current_proc->pgdir = pgdir;
    debug_printf("exec: top is at 0x%x\n", top);
    current_proc->text_size = top - USER_TEXT_BASE;
    current_proc->brk = (void*)top;
    current_proc->tf->eip = eh.entry;
    current_proc->tf->user_esp = sp;
    uvm_switch(current_proc);

    debug_printf("exec: free old pgdir\n");
    uvm_free(old_pgdir);
    old_pgdir  = 0;
    old_pgdir ++;
    asm volatile("sti");

    return 0;

}

int sys_do_exec(const char* path){
    int i;
    char *s, *name;
    uint32_t sz, sp, off, pa, ustack[3 + MAX_ARGC + 1];
    pde_t *pgdir, *old_pgdir;
    struct elf32hdr eh;
    struct proghdr ph;

    debug_printf("exec: try to read form disk...\n");

    pgdir = 0;
    i = off = 0;

    pgdir = (pde_t *)ram_alloc();
    debug_printf("exec: that's addr of user prog PAGE DIR\n");
    kvm_init(pgdir);

    int fd = sys_open(path, 0);
    stat f_st;
    sys_fstat(fd, &f_st);
    auto f_sz = f_st.size;
    if(f_sz > 128 * 512)
    {
        printf("exec: file is too big (%u bytes > 128 * 512 bytes) to load\n", f_sz);
    }
    sys_read(fd, binary_image_buf, f_sz);
//    sys_read_hard_disk(SEL_KERN_DATA, (uint32_t)(char*)(binary_image_buf), n, 40);
//    uint32_t p = 0;
//    p += sizeof(elf32hdr);
    memcpy(&eh, binary_image_buf, sizeof(elf32hdr));
    debug_printf("exec: parsering elf\n");
    print_elfhdr(&eh);

    if (eh.magic != ELF_MAGIC){
        printf("exec: bad executable file\n");
        if (pgdir){
            uvm_free(pgdir);
        }
        return -1;
    }

    debug_printf("exec: load program section to memory\n");

    // load program to memory
    sz = USER_TEXT_BASE;
    for (i = 0, off = eh.phoff; i < eh.phnum; i++, off += sizeof(ph)){
        memcpy(&ph, binary_image_buf + off, sizeof(ph));
//        p += sizeof(ph);
        print_proghdr(&ph);
        if (ph.type != ELF_PROG_LOAD){
            continue;
        }
        if (ph.memsz < ph.filesz){
            printf("exec: bad executable file\n");
            if (pgdir){
                uvm_free(pgdir);
            }
            return -1;
        }
        if ((sz = uvm_alloc(pgdir, sz, ph.vaddr + ph.memsz)) == 0){
            printf("exec: can not alloc user space\n");
            if (pgdir){
                uvm_free(pgdir);
            }
            return -1;
        }
        if (uvm_load(pgdir, ph.vaddr, binary_image_buf, ph.off, ph.filesz) < 0){
            printf("exec: load executable to user space\n");
            if (pgdir){
                uvm_free(pgdir);
            }
            return -1;
        }
//        p += ph.filesz;
    }


    auto top = PAGE_ALIGN_UP(sz);
    debug_printf("exec: build user stack\n");
    /* build user stack */
    sz = USER_BASE;
    if ((sz = uvm_alloc(pgdir, sz, sz + USER_STACK_SIZE)) == 0){
        printf("exec: can not alloc user stack\n");
        if (pgdir){
            uvm_free(pgdir);
        }
        return -1;
    }
    sp = sz;
    if (vmm_get_mapping(pgdir, sz - USER_STACK_SIZE, &pa) == 0){  // sz is no mapped
        printf("exec: can not map user stack\n");
        if (pgdir){
            uvm_free(pgdir);
        }
        return -1;
    }
    pa += USER_STACK_SIZE;
    uint32_t argc = 1;
    ustack[3+argc] = 0;
    ustack[0] = 0xffffffff;
    ustack[1] = argc;
    ustack[2] = sp - (argc+1)*4;  // argv pointer

    sp -= (3 + argc + 1)*4;
    pa -= (3 + argc + 1)*4;
    memcpy((void *)pa, ustack, (3 + argc + 1)*4);   // combine

    debug_printf("exec: prepare for new process `%s`\n", "");

    asm volatile("cli");
    strcpy(current_proc->name, "");

    old_pgdir = current_proc->pgdir;
    current_proc->pgdir = pgdir;
    debug_printf("exec: top is at 0x%x\n", top);
    current_proc->text_size = top - USER_TEXT_BASE;
    current_proc->brk = (void*)top;
    current_proc->tf->eip = eh.entry;
    current_proc->tf->user_esp = sp;
    uvm_switch(current_proc);

    debug_printf("exec: free old pgdir\n");
    uvm_free(old_pgdir);
    old_pgdir  = 0;
    old_pgdir ++;
    asm volatile("sti");

    return 0;

}
