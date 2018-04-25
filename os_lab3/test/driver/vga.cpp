//
// Created by 李新锐 on 23/04/2018.
//

#include "vga.h"
#include "../kernel_lib/sys_utility.h"

void vga_putchar(int c, int color, int x, int y)
{
#ifdef USE_BOCHS_DEBUG_OUTPUT
    sys_dbg_bochs_putc(c);
#endif
    //下面这段C代码会产生movsx指令，因此在bochs的虚拟8086模式中会引发异常
    volatile uint16_t * pVRAM = (volatile uint16_t *) 0xB8000;
    pVRAM = pVRAM + (x * 80 + y) ;
    *pVRAM = c | (color << 8);
}

void vga_scroll_up()
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

void vga_clear_screen()
{
    for(int i = 0; i < 25; ++i)
    {
        for(int j = 0; j <80; ++j)
        {
            vga_putchar(' ', MAKE_COLOR(VGA_BLACK, VGA_WHITE), i, j);
        }
    }
}

void vga_move_cursor(int x, int y)
{
    uint16_t pos = x * 80 + y;
    sys_outb(0x3D4, 0x0F);
    sys_outb(0x3D5, (uint8_t) (pos & 0xFF));
    sys_outb(0x3D4, 0x0E);
    sys_outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}