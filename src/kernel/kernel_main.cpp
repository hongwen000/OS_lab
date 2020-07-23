#include "./tty.h"
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
#include "../fs/fat32.h"
#include "../fs/sys_uio.h"
#include "../driver/ide.h"

extern "C" void interrupt_timer();
extern "C" void interrupt_kb();
extern "C" void interrupt_ide();
extern "C" void interrupt_ide_slave();
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

#ifdef COLORIZE_DEBUG_PRINTF
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
#else
void sys_current_tty_putchar(int ch)
{
    int mask = 0x00FF;
    ch = ch & mask;

    tty* _current_tty = sys_get_current_tty();
    if (_current_tty) _current_tty->putchar(ch);
    sys_dbg_bochs_putc(ch);
}

#endif

const char* str = "Welcome to HHOS version 1.9.0\nYou can input help to see how to use it!";
static inline void print_ok(char * mod) {
    printf("%s init [", mod);
    current_tty->set_color(MAKE_COLOR(VGA_BLACK, VGA_GREEN));
    printf("OK");
    current_tty->set_color(MAKE_COLOR(VGA_BLACK, VGA_WHITE));
    printf("]\n");
}
extern "C" void kb_init();
uint8_t fs_buf[512];
char write_buf[512] = "This is a big big big world\n";
SectorBuffer_t sectorBuffer;
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
    kb_init();
    idt_install(ISR_IRQ0 + IRQ_KB, (uint32_t)interrupt_kb, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    print_ok("Keyboard");
    idt_install(ISR_IRQ0 + IRQ_IDE, (uint32_t)interrupt_ide, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + IRQ_IDE_SLAVE, (uint32_t)interrupt_ide_slave, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    ide_init();
    print_ok("IDE Disk");
    idt_install(0x99, (uint32_t)interrupt_99h, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    idt_install(0x97, (uint32_t)interrupt_97h, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_USER);
    //int 90/91是退出用户程序
    idt_install(0x90, (uint32_t)interrupt_90h, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);
    idt_install(0x91, (uint32_t)interrupt_91h, SEL_KCODE << 3, GATE_INT, IDT_PR | IDT_DPL_KERN);


    set_pit_freq();

    asm volatile("sti");
    // ide_request buffer;  // used for test
    // buffer.dev = 0;
    // buffer.lba= 0;
    // buffer.cmd = B_BUSY;
    // ide_rw(&buffer);
    // ide_test();
//    sys_write_hard_disk(SEL_KERN_DATA, (uint32_t)&write_buf[0], 0, 1);
//    asm volatile("int $0x97");
    auto fs_info = FS_read_info(fs_buf);
    Show_FS_Info(fs_info);
//    FileInfo_t fp;
//    Read_Print_File("/SH.ELF", &fp, fs_info, &sectorBuffer);
//    bochs_break();
    fs_init();
    print_ok("File System");
    ram_init();
    print_ok("Memory");
    vmm_init();
    print_ok("Paging");
    proc_init();
    print_ok("Process");
    printf("Now scheduler launches\n");
    scheduler();
//    sh1.run();
    while(1);
}
