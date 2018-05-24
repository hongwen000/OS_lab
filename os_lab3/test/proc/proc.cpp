//
// Created by 李新锐 on 25/04/2018.
//
#include "./proc.h"
#include "../kernel_lib/page.h"
#include "../libc/stdio.h"
#include "../kernel_lib/debug_printf.h"
#include "../kernel_lib/ram.h"

static PCB ptable[MAX_PROC];
static PCB *initproc = nullptr;
PCB *current_proc = nullptr;
context_t *cpu_context;
static uint32_t cur_pid = 0;
extern void pic_init();
void fork_ret(){
    static int fst = 1;

    if (fst == 1){
        fst = 0;
    } else {
        pic_init();
    }

    return;
}
static PCB *proc_alloc(){
    PCB *pp;
    char *sp;

    for (pp = &ptable[0]; pp < &ptable[MAX_PROC]; pp++){
        /* found */
        if (pp->state == P_UNUSED){
            debug_puts("proc_alloc: found a unused entry\n");

            pp->state = P_USED;
            pp->pid = ++cur_pid;

            /* alloc kernstack */
            pp->kern_stack = (char *)ram_alloc();
            sp = pp->kern_stack + PAGE_SIZE;

            sp -= sizeof(*pp->tf);
            pp->tf = (struct int_frame *)sp;

            sp -= 4;
            *(uint32_t *)sp = (uint32_t)sys_isr_ret;

            sp -= sizeof(*(pp->context));
            pp->context = (struct context_t *)sp;
            memset(pp->context, 0, sizeof(*pp->context));

            pp->context->eip = (uint32_t)fork_ret;

            return pp;
        }
    }
    return 0;
}
void proc_init(){
    struct PCB *pp;
    extern char __init_start;
    extern char __init_end;

    cur_pid = 0;

    debug_puts("proc_userinit: clear ptable\n");

    memset(ptable, 0, sizeof(PCB) * MAX_PROC);

    debug_puts("proc_userinit: prepare for init\n");

    pp = proc_alloc();

    pp->pgdir = (pde_t *)ram_alloc();
    debug_printf("proc_userinit: get pgdir: 0x%x\n", pp->pgdir);

    kvm_init(pp->pgdir);
    vmm_map(pp->pgdir, (uint32_t)pp->kern_stack, (uint32_t)pp->kern_stack, PTE_P | PTE_R | PTE_K);
    debug_puts("proc_userinit: kernel space maped\n");

    uint32_t size = &__init_end - &__init_start;
    pp->text_size = PAGE_SIZE;

    uvm_init_fst(pp->pgdir, &__init_start, size);
    debug_puts("proc_serinit: user space maped\n");

    memset(pp->tf, 0, sizeof(*pp->tf));
    pp->tf->cs = (SEL_UCODE << 3) | 0x3;
    pp->tf->ds = (SEL_UDATA << 3) | 0x3;
    pp->tf->es = pp->tf->ds;
    pp->tf->fs = pp->tf->ds;
    pp->tf->gs = pp->tf->ds;
    pp->tf->ss = pp->tf->ds;
    pp->tf->eflags = FLAG_IF;
    pp->tf->user_esp = USER_TEXT_BASE;
    pp->tf->eip = USER_TEXT_BASE;
    debug_puts("proc_init: init stack build\n");

    strcpy(pp->name, "init");

    pp->state = P_RUNNABLE;

    initproc = pp;

    debug_puts("proc_init: done, perpare for running\n");
}

//the entity of `context` is on stack
extern "C" void sys_context_switch(context_t** p_p_context, context_t* p_context);
void scheduler(){
    PCB *pp;

    debug_puts("scheduler: start\n");

    for (;;){
        for (pp = &ptable[0]; pp < &ptable[MAX_PROC]; pp++){
            asm volatile("cli");
            if (pp->state != P_RUNNABLE){
                continue;
            }

//            debug_printf("scheduler: proc `%s`(PID: %d) will run\n", pp->name, pp->pid);

            uvm_switch(pp);
            pp->state = P_RUNNING;

            current_proc = pp;
//            debug_puts(">>>> context switch\n");
            sys_context_switch(&cpu_context, pp->context);
//            debug_printf("<<<< return form proc `%s`(PID: %d)\n", pp->name, pp->pid);
            asm volatile("sti");
        }
    }
}

void sched(){
    if (current_proc == nullptr) return;
    if (current_proc->state == P_RUNNABLE)
        debug_puts("sched: no runable\n");

    if (current_proc->state == P_RUNNING){
        current_proc->state = P_RUNNABLE;
    }
    sys_context_switch(&current_proc->context, cpu_context);
}

void sys_do_sleep(void *sleep_chain){
    if(current_proc == nullptr)
        debug_puts("sleep: no proc\n");

    debug_printf("sleep: proc `%s`(PID: %d) is going to sleep...\n", current_proc->name, current_proc->pid);
    /* go to sleep
     * NB: This operation MUST be atomic
     * 这是一处确定有死锁风险的代码，当 sleep_chain 赋值后，如果父进程的 wait 马上执行，
     * 那么 wait 将因为找不到任何需要唤醒的进程而退出，此后本进程才进入 P_SLEEPING 状态。
     * 只是这时已经没有进程能够唤醒它了。
     */
    asm volatile("cli");
    current_proc->sleep_chain = sleep_chain;
    current_proc->state = P_SLEEPING;
    asm volatile("sti");

    sched();

    // wake up
    current_proc->sleep_chain = nullptr;

    debug_printf("sleep: proc `%s`(PID: %d)  wakeup...\n", current_proc->name, current_proc->pid);

    // yes, we call pic_init again... :(
    pic_init();
}

void wakeup(void *sleep_chain){
    PCB *pp;

    for (pp = ptable; pp < &ptable[MAX_PROC]; pp++){
        if (pp->state == P_SLEEPING && pp->sleep_chain == sleep_chain){
            pp->state = P_RUNNABLE;
        }
    }
}
int sys_do_wait(){
    int havekids, pid;
    PCB* pp;

    debug_puts("wait: waiting...\n");
    for (;;){
        havekids = 0;
        for (pp = ptable; pp <= &ptable[MAX_PROC]; pp++){
            if (pp->parent != current_proc){
                continue;
            }

            havekids = 1;

            if (pp->state == P_ZOMBIE){
                debug_printf("wait: recycle proc `%s`(PID: %d)\n", pp->name, pp->pid);
                // can be clear
                pid = pp->pid;

                /* free mem */
                ram_free((uint32_t)pp->kern_stack);
                pp->kern_stack = 0;
                uvm_free(pp->pgdir);

                pp->state = P_UNUSED;
                pp->pid = 0;
                pp->parent = 0;
                pp->name[0] = 0;
                pp->killed = 0;

                return pid;
            }
        }

        if (!havekids || current_proc->killed){
            return -1;
        }

        // wait for chidren to exit
        sys_do_sleep(current_proc);
    }
}

void sys_do_exit(){
    PCB *pp;
    int fd;

    if(current_proc == initproc)
        debug_puts("exit: initproc can no exit\n");

    debug_puts("exit: closing opening file\n");

    current_proc->cwd = 0;

    asm volatile("cli");
    //wakeup(proc->parent);

    debug_puts("exit: collecting subprocess\n");
    for (pp = ptable; pp < &current_proc[MAX_PROC]; pp++){
        if (pp->parent == current_proc){
            pp->parent = current_proc->parent;
            if (pp->state == P_ZOMBIE){
                wakeup(current_proc->parent);
            }
        }
    }
    wakeup(current_proc->parent);

    debug_puts("exit: ZOMBIE\n");
    current_proc->state = P_ZOMBIE;
    asm volatile("sti");


    sched();
    debug_puts("exit: return form sched");
    bochs_break();
}
int sys_do_fork(){
    int i;
    PCB *child;

    debug_printf("fork: fork `%s`\n", current_proc->name);

    if ((child = proc_alloc()) == 0){
        return -1;
    }

    debug_puts("fork: copying memory...\n");

    child->pgdir = uvm_copy(current_proc->pgdir, current_proc->text_size);

    if (child->pgdir == 0){
        debug_puts("fork:");
        ram_free((uint32_t)child->kern_stack);
        child->kern_stack = 0;
        child->state = P_UNUSED;
        return -1;
    }

    debug_puts("fork: copying attrib...\n");
    child->text_size = current_proc->text_size;
    child->parent = current_proc;
    *(child->tf) = *(current_proc->tf); // return form same address

    child->tf->eax = 0;
    child->cwd = current_proc->cwd;
    strcpy(child->name, current_proc->name);

    child->state = P_RUNNABLE;

    debug_puts("fork: done\n");
    return child->pid;
}
int sys_do_clone(){
    int i;
    PCB *child;

    debug_printf("fork thread: fork thread `%s`\n", current_proc->name);

    if ((child = proc_alloc()) == 0){
        return -1;
    }

    debug_puts("fork thread: allocating independent stack memory...\n");

    child->pgdir = uvm_copy_thread(current_proc->pgdir, current_proc->text_size);

    if (child->pgdir == 0){
        debug_puts("fork:");
        bochs_break();
        ram_free((uint32_t)child->kern_stack);
        child->kern_stack = 0;
        child->state = P_UNUSED;
        return -1;
    }

    debug_puts("fork thread: copying attrib...\n");
    child->text_size = current_proc->text_size;
    child->parent = current_proc;
    *(child->tf) = *(current_proc->tf); // return form same address

    child->tf->eax = 0;
    child->cwd = current_proc->cwd;
    strcpy(child->name, current_proc->name);

    child->state = P_RUNNABLE;

    debug_puts("fork thread: done\n");
    return child->pid;
}
