#include "sys_lib.h"
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

void sys_print_string(const char* str, unsigned int len, int x, int y)
{
    int color = MAKE_COLOR(VGA_BLACK, VGA_WHITE);
    for(unsigned int i = 0; i < len; ++i)
    {
        sys_putchar(str[i], color, x, y + i);
    }
}

void sys_print_int(int num, int x, int y)
{
    int num_ = num;
    int len = 1;
    while(num_ /= 10) len++;
    char arr[len + 1];
    int power = 1;
    for(int i = 0; i < len; ++i, power *= 10)
        arr[len - i - 1] = '0' + ((num / power) % 10);
    arr[len] = '\0';
    sys_print_string(arr, len, x, y); 
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