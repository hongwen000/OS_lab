//
// Created by 李新锐 on 21/05/2018.
//

#include "sys_proc.h"
#include "../kernel_lib/debug_printf.h"
#include "../kernel_lib/page.h"
#include "../kernel_lib/ram.h"

int sys_fork(){
    return sys_do_fork();
}

int sys_wait(){
    return sys_do_wait();
}

int sys_exit(){
    debug_printf("sys_exit: proc `%s`(PID: %d) exit\n", current_proc->name, current_proc->pid);
    sys_do_exit();
    return 0; // no reach
}

//int sys_exec(){
//    char *path, *argv[MAX_ARGC];
//    int i;
//    uint32_t uargv, uarg; // pointer to argv[]
//
//    if (argstr(0, &path) < 0 || argint(1, (int *)&uargv) < 0){
//        return -1;
//    }
//
//    printl("sys_exec: get path %s argv: 0x%x\n", path, uargv);
//
//    memset(argv, 0, sizeof(argv));
//
//    for (i = 0; ; i++){
//        if (i >= MAX_ARGC){
//            return -1;  // too many arguments
//        }
//        if (fetchint(uargv + 4*i, (int *)&uarg) < 0){
//            return -1;
//        }
//        if (uarg == 0){
//            argv[i] = 0;
//            break;
//        }
//        if (fetchstr(uarg, &argv[i]) < 0){
//            return -1;
//        }
//    }
//
//    return exec(path, argv);
//}

//int sys_kill(){
//    int pid;
//
//    if (argint(0, &pid) < 0){
//        return -1;
//    }
//
//    printl("sys_kill: param: pid = %d\n", pid);
//    return kill(pid);
//}

//int sys_getpid(){
//    return proc->pid;
//}

extern uint32_t HHOS_timer_ticks;
int sys_sleep(int n){
    int trick0;

    debug_printf("sys_sleep: proc `%s`(PID: %d)\n", current_proc->name, current_proc->pid);

    debug_printf("sys_sleep: sleep for %d0ms\n", n);

    trick0 = HHOS_timer_ticks;
    while ((int)HHOS_timer_ticks - trick0 < n){

        debug_printf("sys_sleep: proc `%s`(PID: %d) is still sleeping...\n", current_proc->name, current_proc->pid);

        if (current_proc->killed){
            return -1;
        }
        sys_do_sleep(&HHOS_timer_ticks);
    }

    debug_printf("sys_sleep: proc `%s`(PID: %d) weakup, acutal time: %d0ms\n", current_proc->name, current_proc->pid, HHOS_timer_ticks - trick0);
    return 0;
}

int sys_clone() {
    return sys_do_clone();
}

int sys_exec(uint32_t n)
{
    return sys_do_exec(n);
}

int sys_p(int sem_id) {
//    debug_printf("sys_p: sem_id = %d\n", sem_id);
    return sys_do_p(sem_id);
}

int sys_v(int sem_id) {
    return sys_do_v(sem_id);
}

int sys_getsem(int v) {
    return sys_do_getsem(v);
}

int sys_freesem(int sem_id) {
    return sys_do_freesem(sem_id);
}

void *sys_sbrk(int len) {
    auto old_size = USER_TEXT_BASE + current_proc->text_size;
    auto new_size = USER_TEXT_BASE + PAGE_ALIGN_UP(current_proc->text_size + len);
    debug_printf("sys_sbrk: extending memory from 0x%x to 0x%x\n", old_size, new_size);
    uvm_alloc(current_proc->pgdir, old_size, new_size);
    current_proc->text_size = PAGE_ALIGN_UP(current_proc->text_size + len);
    return (void*)old_size;
}

int sys_munmap(void *addr, size_t length) {
    for(uint32_t p = PAGE_ALIGN_DOWN((uint32_t)addr); p < PAGE_ALIGN_UP((uint32_t)addr+ length); p += PAGE_SIZE)
    {
        uint32_t pa;
        vmm_get_mapping(current_proc->pgdir, (uint32_t)addr, &pa);
        debug_printf("sys_munmap: free pa 0x%x va 0x%x\n", p, pa);
        ram_free(pa);
    }
    return 0;
}

//int sys_uptime(){
//    return timer_ticks;
//}
