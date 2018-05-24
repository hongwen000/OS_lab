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
#include "../kernel_lib/debug_printf.h"

extern "C" void interrupt_timer();
extern "C" void interrupt_kb();
extern "C" void interrupt_ide();
extern "C" void interrupt_90h();
extern "C" void interrupt_91h();
extern "C" void interrupt_97h();
extern "C" void interrupt_99h();
extern "C" void system_call_asm();
extern "C" void interrupt_timer();
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
    sys_dbg_bochs_putc('\e');
    sys_dbg_bochs_putc('[');
    sys_dbg_bochs_putc('1');
    sys_dbg_bochs_putc(';');
    sys_dbg_bochs_putc('3');
    sys_dbg_bochs_putc('4');
    sys_dbg_bochs_putc('m');
    sys_dbg_bochs_putc(ch);
    sys_dbg_bochs_putc('\e');
    sys_dbg_bochs_putc('[');
    sys_dbg_bochs_putc('0');
    sys_dbg_bochs_putc('m');
}
const char* str = "Welcome to HHOS version 1.2.0\nYou can input help to see how to use it!";
static inline void print_ok(char * mod) {
    printf("%s init [", mod);
    current_tty->set_color(MAKE_COLOR(VGA_BLACK, VGA_GREEN));
    printf("OK");
    current_tty->set_color(MAKE_COLOR(VGA_BLACK, VGA_WHITE));
    printf("]\n");
}
sh* p_sh = nullptr;
extern "C" void run_sh()
{
    p_sh->run();
}
extern "C" void kernel_main()
{
    real_world_cls();
    idt_install(ISR_SYSCALL, (uint32_t)system_call_asm, SEL_KCODE << 3, GATE_TRAP, IDT_PR | IDT_DPL_USER);
    tty tty1(false);
    tty1.set_x(6);
    current_tty = &tty1;
    print_ok("TTY");
    idt_install(ISR_IRQ0 + IRQ_TIMER, (uint32_t)interrupt_timer, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    print_ok("Clock");
    idt_install(ISR_IRQ0 + IRQ_KB, (uint32_t)interrupt_kb, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    print_ok("Keyboard");
    idt_install(ISR_IRQ0 + IRQ_IDE, (uint32_t)interrupt_ide, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    print_ok("IDE Disk");
    idt_install(0x99, (uint32_t)interrupt_99h, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    idt_install(0x97, (uint32_t)interrupt_97h, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_USER);
    //int 90/91是退出用户程序
    idt_install(0x90, (uint32_t)interrupt_90h, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    idt_install(0x91, (uint32_t)interrupt_91h, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);


    set_pit_freq();

    asm volatile("sti");
//    asm volatile("int $0x97");
    ram_init();
    print_ok("Detect physical memory");
    vmm_init();
    print_ok("Paging");
    proc_init();
    print_ok("Process");
    scheduler();

    sh sh1;
    print_ok("Shell");
    printf("%s\n", str);
    p_sh = &sh1;
//    sh1.run();
    while(1);
}
