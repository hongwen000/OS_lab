#include "sys_lib.h"
#include "../kernel/kb.h"
#include "../kernel/ide.h"

extern "C" void sys_bios_print_string(const char* str, unsigned int len, int color, int pos);
extern "C" int sys_getchar()
{
    unsigned char ch = kb_buf_out();
    return ch;
}

void sys_bios_print_int(int num, int color, int pos)
{
    int num_ = num;
    int len = 1;
    while(num_ /= 10) len++;
    char arr[len + 1];
    int power = 1;
    for(int i = 0; i < len; ++i, power *= 10)
        arr[len - i - 1] = '0' + ((num / power) % 10);
    arr[len] = '\0';
    sys_bios_print_string(arr, len, color, pos);
}

void sys_print_string(const char* str, unsigned int len, int color, int pos)
{
    uint8_t x = (pos >> 8) & 0xFF;
    uint8_t y = (pos & 0xFF);
    for(unsigned int i = 0; i < len; ++i)
    {
        sys_putchar(str[i], color, x, y + i);
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
//void sys_putchar(char c, int color, int x, int y)
//{
    ////下面这段C代码会产生movsx指令，即使是实模式也会引发异常
    ////volatile uint16_t * pVRAM = (volatile uint16_t *) 0xB8000;
    ////pVRAM = pVRAM + (y * 80 + x) ;
    ////*pVRAM = c | (color << 8);
//


    //uint16_t _bx = 0xB800;
    //uint16_t _si = (80 * x + y) * 2;
    //uint16_t _ax = c | (color << 8);
//
    //asm volatile(".intel_syntax noprefix\n\t"
                   ////"mov es, bx\n\t"
                   ////"mov word ptr es:[si], ax\n\t"
    //"mov ax, 0xB800\n\t"
    //"mov gs, ax\n\t"
    //"xor ax,ax\n\t"
    //"mov ax,1\n\t"
	//"mov bx,80\n\t"
	//"mul bx\n\t"
	//"add ax,1\n\t"
	//"mov bx,2\n\t"
	//"mul bx\n\t"
	//"mov bp,ax\n\t"
	//"mov ah,1\n\t"
	//"mov al,41\n\t"
    //"mov word ptr gs:[bp],ax\n\t"
                   //".att_syntax noprefix\n\t");
//}

void sys_putchar(int c, int color, int x, int y)
{
#ifdef USE_BOCHS_DEBUG_OUTPUT
    sys_dbg_bochs_putc(c);
#endif
    int offset = (x * 80 + y)*2;
    uint16_t ch = c | (color << 8);
    asm volatile (".intel_syntax noprefix\n\t"
    "pusha\n\t"
    "mov cx, 0xB800\n\t"
    "mov gs, cx\n\t"
	"mov bp,bx\n\t"
    "mov word ptr gs:[bp],ax\n\t"
    "popa\n\t"
    ".att_syntax noprefix\n\t"
    :
    :"a"(ch),"b"(offset)
    :"%ecx");
}

void sys_scroll_up()
{
    asm volatile(
    "cld\n\t"
    "movl $0xF00, %%ecx\n\t"
    "movl $0x0, %%esi\n\t"
    "scroll_up_loop:\n\t"
    "movb %%gs:0xA0(%%esi), %%al\n\t"
    "movb %%al, %%gs:(%%esi)\n\t"
    "inc %%esi\n\t"
    "loop scroll_up_loop\n\t"
    :
    :
    :"%esi", "%al", "%ecx"
    );
}

void sys_clear_screen()
{
    for(int i = 0; i < 25; ++i)
    {
        for(int j = 0; j <80; ++j)
        {
            sys_putchar(' ', MAKE_COLOR(VGA_BLACK, VGA_WHITE), i, j);
        }
    }
}
void sys_bios_scroll_up(int color)
{
    //这里pusha和popa是必要的，不然会错误！！！（一小时）
    asm volatile
    ("pusha\n\t"
    "movb $1, %%al\n\t"
    "movb %0, %%bh\n\t"
    "movb $0, %%ch\n\t"
    "movb $0, %%cl\n\t"
    "movb $24, %%dh\n\t"
    "movb $79, %%dl\n\t"
    "movb $0x06, %%ah\n\t"
    "int $0x10\n\t"
    "popa\n\t"
    :
    : "g"(color));
}

void sys_bios_putchar(char c, int color, int x, int y)
{
    sys_bios_print_string(&c, 1, color, MAKE_POS(x, y));
}

uint8_t sys_inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

//uint8_t sys_inb_intel(uint16_t port)
//{
//    uint8_t ret;
//    asm(".intel_syntax noprefix\n");  
//    asm volatile ( "inb al, dx"
//                   : "=a"(ret)
//                   : "Nd"(port) );
//    asm(".att_syntax noprefix\n");
//    return ret;
//}

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

// uint8_t sys_inb(uint16_t port)
// {
//     uint8_t ret;
//     asm volatile ( "inb al, dx"         //*****这而我当时错了，计组汇编诶
//                    : "=a"(ret)
//                    : "Nd"(port) );
//     return ret;
// }

void sys_dbg_bochs_putc(char c){
    sys_outb(0xe9, (uint8_t)c);
}


char sys_get_scancode()
{
    while (!(sys_inb(0x64) & 1));
    return sys_inb(0x60);
}

void sys_read_disk(uint32_t segment, uint32_t address, uint16_t logical_start_sector, uint8_t secotr_cnt)
{
    uint8_t head = (logical_start_sector % (FLOPPY_HEAD_PER_DISK * FLOPPY_SECTOR_PER_TRACK)) / FLOPPY_SECTOR_PER_TRACK;
    uint16_t cylinder = logical_start_sector / (FLOPPY_HEAD_PER_DISK * FLOPPY_SECTOR_PER_TRACK);
    uint16_t sector = (logical_start_sector % (FLOPPY_HEAD_PER_DISK * FLOPPY_SECTOR_PER_TRACK)) % FLOPPY_SECTOR_PER_TRACK + 1;
    //位操作一定要注意类型匹配（两小时）
    uint16_t mask1 = 255;
    uint16_t mask2 = 768;
    uint16_t _cx = ((cylinder & mask1) << 8) | ((cylinder & mask2) >> 2) | sector;
    asm volatile
    ("push %%es\n\t"
     "movw %0, %%bx\n\t"
     "movw %%bx, %%es\n\t"  //段地址
     "movw %1, %%bx\n\t"    //加载地址
     "movb %2, %%dh\n\t"    //head号
     "movw %3, %%cx\n\t"    //cx高十位是cylinder号，低8位是sector号
     "movb %4, %%al\n\t"    //扇区数
     "movb $0, %%dl\n\t"    //驱动器号
     "movb $2, %%ah\n\t"    //功能号
     "int $0x13\n\t"
     "pop %%es\n\t"
     : :"g"(segment), "g"(address), "g"(head), "g"(_cx), "g" (secotr_cnt));
}
void sys_read_hard_disk(uint32_t segment, uint32_t address, uint16_t logical_start_sector, uint8_t secotr_cnt)
{
    ide_request buf;
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
}
extern "C" void interrupt_33h_c() {
    const char* l11 = " _____      _     _____  _____ ";
    const char* l12 = "|_   _|    | |   |____ ||____ |";
    const char* l13 = "  | | _ __ | |_      / /    / /";
    const char* l14 = "  | || '_ \\| __|     \\ \\    \\ \\";
    const char* l15 = " _| || | | | |_  .___/ /.___/ /";
    const char* l16 = " \\___/_| |_|\\__| \\____/ \\____/ ";
    sys_print_string(l11, 31, MAKE_COLOR(VGA_BLACK, VGA_CYAN), MAKE_POS(0, 0));
    sys_print_string(l12, 31, MAKE_COLOR(VGA_BLACK, VGA_CYAN), MAKE_POS(1, 0));
    sys_print_string(l13, 31, MAKE_COLOR(VGA_BLACK, VGA_CYAN), MAKE_POS(2, 0));
    sys_print_string(l14, 31, MAKE_COLOR(VGA_BLACK, VGA_CYAN), MAKE_POS(3, 0));
    sys_print_string(l15, 31, MAKE_COLOR(VGA_BLACK, VGA_CYAN), MAKE_POS(4, 0));
    sys_print_string(l16, 31, MAKE_COLOR(VGA_BLACK, VGA_CYAN), MAKE_POS(5, 0));
}

extern "C" void interrupt_34h_c() {

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
