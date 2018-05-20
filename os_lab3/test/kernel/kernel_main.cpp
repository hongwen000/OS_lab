#include "./tty.h"
#include "./sh.h"
#include "./bin_loader.h"
#include "../include/defines.h"
#include "../kernel_lib/sys_utility.h"
#include "../libc/string.h"
#include "../libc/stdio.h"
#include "../libc/sys/hhos.h"
#include "../kernel_lib/pm.h"
#include "../kernel_lib/isr.h"
#include "../proc/proc.h"
#include "../kernel_lib/ram.h"
#include "../kernel_lib/page.h"
#include "../libc/assert.h"

extern "C" void interrupt_timer();
extern "C" void interrupt_kb();
extern "C" void interrupt_ide();
extern "C" void interrupt_90h();
extern "C" void interrupt_91h();
extern "C" void interrupt_97h();
extern "C" void interrupt_99h();
extern "C" void interrupt_system_call();
extern "C" void sys_proc_schd();
extern "C" void set_pit_freq();
extern "C" void gui_test();
static tty* current_tty = nullptr;
tty* sys_get_current_tty(){return current_tty;}

int  sys_current_tty_get_color()
{
    tty* _current_tty = sys_get_current_tty();
    if (_current_tty) return _current_tty->get_color();
    return MAKE_COLOR(VGA_BLACK, VGA_WHITE);
}

void sys_current_tty_set_color(int color)
{
    tty* _current_tty = sys_get_current_tty();
    if (_current_tty) _current_tty->set_color(color);
}

void sys_current_tty_putchar(int ch)
{
    int mask = 0x00FF;
    ch = ch & mask;

    tty* _current_tty = sys_get_current_tty();
    if (_current_tty) _current_tty->putchar(ch);
    sys_dbg_bochs_putc(ch);
}
const char* str = "Welcome to HHOS version 1.2.0\nYou can input help to see how to use it!";
static inline void print_ok(char * mod) {
    printf("%s init [", mod);
    current_tty->set_color(MAKE_COLOR(VGA_BLACK, VGA_GREEN));
    printf("OK");
    current_tty->set_color(MAKE_COLOR(VGA_BLACK, VGA_WHITE));
    printf("]\n");
}

extern "C" void kernel_main()
{
    idt_install(ISR_SYSCALL, (uint32_t)interrupt_system_call, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    tty tty1(false);
    tty1.set_x(6);
    current_tty = &tty1;
    print_ok("TTY");
    idt_install(ISR_IRQ0 + IRQ_TIMER, (uint32_t)sys_proc_schd, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    print_ok("Clock");
    idt_install(ISR_IRQ0 + IRQ_KB, (uint32_t)interrupt_kb, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    print_ok("Keyboard");
    idt_install(ISR_IRQ0 + IRQ_IDE, (uint32_t)interrupt_ide, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    print_ok("IDE Disk");
    idt_install(0x99, (uint32_t)interrupt_99h, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    idt_install(0x97, (uint32_t)interrupt_97h, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    //int 90/91是退出用户程序
    idt_install(0x90, (uint32_t)interrupt_90h, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    idt_install(0x91, (uint32_t)interrupt_91h, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    //int 0x92是新建进程
    idt_install(0x92, (uint32_t)sys_new_proc, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    //int 0x93切换进程
    idt_install(0x93, (uint32_t)sys_proc_schd, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);


    set_pit_freq();

    asm volatile("sti");
    asm volatile("int $0x97");
    ram_init();
    print_ok("Detect physical memory");
    vmm_init();
    print_ok("Paging");
    proc_init();
    print_ok("Process Management");

//    sh sh1;
//    print_ok("Shell");
//    printf("%s\n", str);
//    sh1.run();
    while(1);
}
