//
// Created by 李新锐 on 23/04/2018.
//

#ifndef HHOS_VGA_H
#define HHOS_VGA_H

#include "../include/defines.h"
void vga_putchar(int c, int color, int x, int y);
void vga_scroll_up();
void vga_clear_screen();
void vga_move_cursor(int x, int y);

#endif //HHOS_VGA_H
