#include "sys_utility.h"
#include "../driver/kb.h"
#include "../driver/ide.h"
#include "../driver/vga.h"
#include "debug_printf.h"
#include "../kernel/tty.h"
#include "../proc/sys_proc.h"
#include "../fs/sys_uio.h"
#include "../libc/stdio.h"

HHOS_info_t HHOS_info;

static char *fault_msg[] = {
        "Division By Zero",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Into Detected Overflow",
        "Out of Bounds",
        "Invalid Opcode",
        "No Coprocessor",

        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault",
        "Page Fault",
        "Unknown Interrupt",

        "Coprocessor Fault",
        "Alignment Check",
        "Machine Check",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",

        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved"
};

void blue_screen(int_frame *r)
{

//    tty blue_tty(true);
//    blue_tty.set_color(MAKE_COLOR(VGA_BLUE, VGA_WHITE));
//    asm volatile("cli\n\t"
//                 "movw $0x18, %ax\n\t"
//                 "movw %ax, %gs"
//                 );
//    for(int i = 0; i < 1999; ++i)
//        tty_debug_printf(blue_tty ," ");
//    blue_tty.set_x(0);
//    blue_tty.set_y(0);
//    tty_debug_printf(blue_tty,"  #          ##\n");
//    tty_debug_printf(blue_tty," ###        #  \n");
//    tty_debug_printf(blue_tty,"  #        #   \n");
//    tty_debug_printf(blue_tty,"     ##### #   \n");
//    tty_debug_printf(blue_tty,"  #        #   \n");
//    tty_debug_printf(blue_tty," ###        #  \n");
//    tty_debug_printf(blue_tty,"  #          ##\n");
//    if(r == nullptr)
//    {
//        tty_debug_printf(blue_tty, "Function not implemented");
//    }
//    if (r->int_no < ISR_IRQ0 && r->int_no >= 0)
//    {
//        tty_debug_printf(blue_tty, "Encountered x86 Exception %d : %s\n", r->int_no, fault_msg[r->int_no]);
//    }
//    else
//    {
//        tty_debug_printf(blue_tty, "Encountered untreated interrupt %d\n", r->int_no);
//    }
//    tty_debug_printf(blue_tty,"Error code: %d\n", r->err_code);
//    tty_debug_printf(blue_tty,"CS:EIP: %d:0x%x\n", r->cs, r->eip);
//    tty_debug_printf(blue_tty,"System will halt");

//    printf("Error code: %d\n", r->err_code);
//    printf("CS:EIP: %d:0x%x\n", r->cs, r->eip);
//    printf("System will halt");

}

extern "C" int sys_getchar()
{
    unsigned char ch = 0;
    ch = kb_buf_out();
    // debug_printf("DEBUG: sys_getchar got %c (%d)\n", ch, (int) ch);
    return ch;
}

void sys_clear_screen()
{
    vga_clear_screen();
}

void sys_putchar(int c, int color, int x, int y)
{
    vga_putchar(c, color, x, y);
}

void sys_scroll_up()
{
    vga_scroll_up();
}

void sys_move_cursor(int x, int y)
{
    vga_move_cursor(x, y);
}

void sys_print_string(const char* str, unsigned int len, int color, int pos)
{
    uint8_t x = (pos >> 8) & 0xFF;
    uint8_t y = (pos & 0xFF);
    for(unsigned int i = 0; i < len; ++i)
    {
        vga_putchar(str[i], color, x, y + i);
    }
}

void sys_print_int(int num, int color, int pos)
{
    int num_ = num;
    int len = 1;
    while(num_ /= 10) len++;
    char arr[len + 1];
    int power = 1;
    for(int i = 0; i < len; ++i, power *= 10)
        arr[len - i - 1] = '0' + ((num / power) % 10);
    arr[len] = '\0';
    sys_print_string(arr, len, color, pos);
}

uint8_t sys_inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}


void sys_outb(uint16_t port, uint8_t data)
{
    asm volatile ( "outb %0, %1"
                   : 
                   : "a"(data), "Nd"(port) );
}

void sys_outsl(uint32_t port, const void *addr, uint32_t cnt)
{
    asm volatile ("cld; rep outsl"
    : "=S" (addr), "=c" (cnt)
    : "d" (port), "0" (addr), "1" (cnt)
    : "cc"
    );
}

void sys_insl(uint32_t port, void *addr, uint32_t cnt){
    asm volatile ("cld; rep insl"
    : "=D" (addr), "=c" (cnt)
    : "d" (port), "0" (addr), "1" (cnt)
    : "memory", "cc"
    );
}


void sys_outw(uint16_t port, uint16_t data)
{
    asm volatile ("outw %0, %1"
                  :
                  :"a"(data), "Nd"(port));
}

void sys_dbg_bochs_putc(char c){
    sys_outb(0xe9, (uint8_t)c);
}


char sys_get_scancode()
{
    while (!(sys_inb(0x64) & 1));
    return sys_inb(0x60);
}

void sys_read_hard_disk(uint32_t segment, uint32_t address, uint16_t logical_start_sector, uint8_t secotr_cnt)
{
    ide_request buf;
    debug_printf("Requested to read %d blocks from lba %d to address %u\n", (int)secotr_cnt, (int) logical_start_sector, address);
    for (int i = logical_start_sector; i < logical_start_sector + secotr_cnt; ++i) {
        buf.reset(i);
        ide_rw(&buf);
#ifdef IDE_TEST
        ide_print_blk(&buf);
#endif
        asm volatile(
        "cld\n\t"
                "movl $0x200, %%ecx\n\t"
                "push %%es\n\t"
                "movw %0, %%es\n\t"
                "movl %1, %%edi\n\t"
                "movl %2, %%esi\n\t"
                "rep movsb\n\t"
                "pop %%es\n\t"
        :
        :"r"(segment), "r"(address), "r"(buf.buf)
        :"%esi", "%edi", "%ecx"
        );
        address += 0x200;
    }
    debug_printf("Read block OK\n");
}

void sys_write_hard_disk(uint32_t segment, uint32_t address, uint16_t logical_start_sector, uint8_t secotr_cnt)
{
    ide_request buf;
    debug_printf("Requested to write %d blocks from address %u to lba %d\n", (int)secotr_cnt, address, (int) logical_start_sector);
    for (int i = logical_start_sector; i < logical_start_sector + secotr_cnt; ++i) {
        asm volatile(
        "cld\n\t"
        "movl $0x200, %%ecx\n\t"
        "push %%es\n\t"
        "movw %0, %%es\n\t"
        "movl %2, %%edi\n\t"
        "movl %1, %%esi\n\t"
        "rep movsb\n\t"
        "pop %%es\n\t"
        :
        :"r"(segment), "r"(address), "r"(buf.buf)
        :"%esi", "%edi", "%ecx"
        );
        address += 0x200;
        buf.prepare_write(i);
        ide_rw(&buf);
#ifdef IDE_TEST
        ide_print_blk(&buf);
#endif
    }
    debug_printf("Write block OK\n");
}

//extern "C" void interrupt_33h_c() {
//    const char* l11 = " _____      _     _____  _____ ";
//    const char* l12 = "|_   _|    | |   |____ ||____ |";
//    const char* l13 = "  | | _ __ | |_      / /    / /";
//    const char* l14 = "  | || '_ \\| __|     \\ \\    \\ \\";
//    const char* l15 = " _| || | | | |_  .___/ /.___/ /";
//    const char* l16 = " \\___/_| |_|\\__| \\____/ \\____/ ";
//    sys_print_string(l11, 31, MAKE_COLOR(VGA_BLACK, VGA_CYAN), MAKE_POS(0, 0));
//    sys_print_string(l12, 31, MAKE_COLOR(VGA_BLACK, VGA_CYAN), MAKE_POS(1, 0));
//    sys_print_string(l13, 31, MAKE_COLOR(VGA_BLACK, VGA_CYAN), MAKE_POS(2, 0));
//    sys_print_string(l14, 31, MAKE_COLOR(VGA_BLACK, VGA_CYAN), MAKE_POS(3, 0));
//    sys_print_string(l15, 31, MAKE_COLOR(VGA_BLACK, VGA_CYAN), MAKE_POS(4, 0));
//    sys_print_string(l16, 31, MAKE_COLOR(VGA_BLACK, VGA_CYAN), MAKE_POS(5, 0));
//}

extern PCB* current_proc;
extern "C" void interrupt_97h_c() {

    debug_printf(">>>>>>> syscall number: %d, form `%s`(PID: %d)\n", current_proc->tf->eax, current_proc->name, current_proc->pid);
    const char * l11 = " __    __  __    __   ______    ______  ";
    const char * l12 = "|  \\  |  \\|  \\  |  \\ /      \\  /      \\ ";
    const char * l13 = "| $$  | $$| $$  | $$|  $$$$$$\\|  $$$$$$\\";
    const char * l14 = "| $$__| $$| $$__| $$| $$  | $$| $$___\\$$";
    const char * l15 = "| $$    $$| $$    $$| $$  | $$ \\$$    \\ ";
    const char * l16 = "| $$$$$$$$| $$$$$$$$| $$  | $$ _\\$$$$$$\\";
    const char * l17 = "| $$  | $$| $$  | $$| $$__/ $$|  \\__| $$";
    const char * l18 = "| $$  | $$| $$  | $$ \\$$    $$ \\$$    $$";
    const char * l19 = " \\$$   \\$$ \\$$   \\$$  \\$$$$$$   \\$$$$$$ ";

    sys_print_string(l11, 40, MAKE_COLOR(VGA_BLACK, VGA_BRIGHT_CYAN), MAKE_POS(0, 40));
    sys_print_string(l12, 40, MAKE_COLOR(VGA_BLACK, VGA_BRIGHT_CYAN), MAKE_POS(1, 40));
    sys_print_string(l13, 40, MAKE_COLOR(VGA_BLACK, VGA_BRIGHT_CYAN), MAKE_POS(2, 40));
    sys_print_string(l14, 40, MAKE_COLOR(VGA_BLACK, VGA_BRIGHT_CYAN), MAKE_POS(3, 40));
    sys_print_string(l15, 40, MAKE_COLOR(VGA_BLACK, VGA_BRIGHT_CYAN), MAKE_POS(4, 40));
    sys_print_string(l16, 40, MAKE_COLOR(VGA_BLACK, VGA_BRIGHT_CYAN), MAKE_POS(5, 40));
    sys_print_string(l17, 40, MAKE_COLOR(VGA_BLACK, VGA_BRIGHT_CYAN), MAKE_POS(6, 40));
    sys_print_string(l18, 40, MAKE_COLOR(VGA_BLACK, VGA_BRIGHT_CYAN), MAKE_POS(7, 40));
    sys_print_string(l19, 40, MAKE_COLOR(VGA_BLACK, VGA_BRIGHT_CYAN), MAKE_POS(8, 40));
}

void system_call_c(int_frame* tf)
{
    uint32_t n = tf->eax;
    uint8_t ah = (n >> 8) & 0xFF;
    uint8_t al = n & 0xFF;
    if (ah == 0)
    {
        tf->eax = sys_getchar();
    }
    else if (ah == 1)
    {
         sys_current_tty_putchar(al);
    }
    else if (ah == 2)
    {
        //Sleep
    }
    else if (ah == 3)
    {
        //Timestamp
    }
    else if (ah == 4)
    {
        //Fork
        tf->eax = sys_fork();
    }
    else if (ah == 5)
    {
        tf->eax = sys_wait();
    }
    else if (ah == 6)
    {
        tf->eax = sys_exit();
    }
    else if (ah == 7)
    {
        tf->eax = sys_clone();
    }
    else if (ah == 8)
    {
        const char *path = *(const char **)(current_proc->tf->user_esp+4);
        debug_printf("system_call: exec : %s\n", path);
        tf->eax = sys_exec(path);
//        uint32_t blk = *(uint32_t*)(current_proc->tf->user_esp+4);
//        debug_printf("sys_exec: +4 : 0x%x = %d\n", (current_proc->tf->user_esp+4), *(int*)(current_proc->tf->user_esp+4));
//        debug_printf("sys_exec: +8 : 0x%x = %d\n", (current_proc->tf->user_esp+8), *(int*)(current_proc->tf->user_esp+8));
//        debug_printf("sys_exec: Exec program in block %d\n", blk);
//        tf->eax = sys_exec(blk);
    }
    else if (ah == 9)
    {
        int arg = *(uint32_t*)(current_proc->tf->user_esp+0);
        tf->eax = sys_p(arg);
    }
    else if (ah == 10)
    {
        int arg = *(uint32_t*)(current_proc->tf->user_esp+0);
        tf->eax = sys_v(arg);
    }
    else if (ah == 11)
    {
        int arg = *(uint32_t*)(current_proc->tf->user_esp+0);
        tf->eax = sys_getsem(arg);
    }
    else if (ah == 12)
    {
        int arg = *(uint32_t*)(current_proc->tf->user_esp+0);
        tf->eax = sys_freesem(arg);
    }
    else if (ah == 13)
    {
        size_t arg = *(size_t *)(current_proc->tf->user_esp+0);
        tf->eax = (uint32_t)sys_sbrk(arg);
    }
    else if (ah == 14)
    {
        void* addr = *(void **)(current_proc->tf->user_esp+0);
        size_t length = *(size_t *)(current_proc->tf->user_esp+4);
        debug_printf("system_call munmap: +0 : 0x%x = 0x%x\n", (current_proc->tf->user_esp+0), *(uint32_t *)(current_proc->tf->user_esp+0));
        debug_printf("system_call munmap: +4 : 0x%x = 0x%x\n", (current_proc->tf->user_esp+4), *(uint32_t *)(current_proc->tf->user_esp+4));
        tf->eax = sys_munmap(addr, length);
    }
    else if (ah == 15)
    {
        const char *path = *(const char **)(current_proc->tf->user_esp+0);
        uint32_t mode = *(uint32_t *)(current_proc->tf->user_esp+4);
        debug_printf("system_call open: +0 : 0x%x = 0x%x\n", (current_proc->tf->user_esp+0), *(uint32_t *)(current_proc->tf->user_esp+0));
        debug_printf("system_call open: +4 : 0x%x = 0x%x\n", (current_proc->tf->user_esp+4), *(uint32_t *)(current_proc->tf->user_esp+4));
        tf->eax = sys_open(path, mode);
    }
    else if (ah == 16)
    {
        int fildes = *(int*)(current_proc->tf->user_esp+0);
        void *buf = *(void **)(current_proc->tf->user_esp+4);
        size_t nbyte = *(size_t *)(current_proc->tf->user_esp+8);
        tf->eax = sys_read(fildes, buf, nbyte);
    }
    else if (ah == 17)
    {
        int fildes = *(int*)(current_proc->tf->user_esp+0);
        void *buf = *(void **)(current_proc->tf->user_esp+4);
        size_t nbyte = *(size_t *)(current_proc->tf->user_esp+8);
        tf->eax = sys_write(fildes, buf, nbyte);
    }
    else if (ah == 18)
    {
        int arg = *(uint32_t*)(current_proc->tf->user_esp+0);
        tf->eax = sys_close(arg);
    }
    else if (ah == 19)
    {
        int arg = *(uint32_t*)(current_proc->tf->user_esp+0);
        stat* buf = *(stat**)(current_proc->tf->user_esp+4);
        tf->eax = sys_fstat(arg, buf);
    }
    else if (ah == 20)
    {
        int fildes = *(int*)(current_proc->tf->user_esp+0);
        int offset = *(int*)(current_proc->tf->user_esp+4);
        int whence = *(int*)(current_proc->tf->user_esp+8);
        tf->eax = sys_lseek(fildes, offset, whence);
    }
    else if (ah == 28)
    {
        sys_clear_screen();
    }
    else if (ah == 29)
    {
        extern tty* sys_get_current_tty();
        sys_get_current_tty()->tty_init();
    }
}

void interrupt_timer_c()
{
    ++HHOS_timer_ticks;
    if (!current_proc) return;

    if (current_proc->killed && (current_proc->tf->cs & CPL_USER) == CPL_USER){
        sys_do_exit();
    }

    wakeup(&HHOS_timer_ticks);

    if (HHOS_timer_ticks % 1000 == 0){
       debug_printf("timer_handler: timer alive, trick: %d\n", HHOS_timer_ticks);
    }

    if(HHOS_timer_ticks % 50 == 0)
        go_back_scheduler();
}
