#ifndef _TTY_H_
#define _TTY_H_
#include "../basic_lib/sys_lib.h"
#include "string.h"

class tty{

private: 
    int cur_x;
    int cur_y;
    int old_x;
    int old_y;
    int color;
    char tty_mem[80*25*2];
public:
    void tty_init()
    {
        cur_x = 0;
        cur_y = 0;
        color = MAKE_COLOR(VGA_BLACK, VGA_WHITE);
        memset(tty_mem, ' ', 80*25*2);
    }
    int  get_x()
    {
        return cur_x;
    }
    int  get_y()
    {
        return cur_y;
    }
    int  get_color()
    {
        return color;
    }
    void set_x(int x)
    {
        cur_x = x;
    }
    void set_y(int y)
    {
        cur_y = y;
    }
    void set_color(int color)
    {
        color = color;
    }
    void move_cursor(int x, int y)
    {
        uint16_t pos = x * 80 + y;
        sys_outb(0x3D4, 0x0F);
        sys_outb(0x3D5, (uint8_t) (pos & 0xFF));
        sys_outb(0x3D4, 0x0E);
        sys_outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
    }
    void putchar_worker(int c, int color, int x, int y)
    {
        tty_mem[y + 80 * x] = c;
        tty_mem[y + 80 * x + 1] = color;
        sys_putchar(c, color, x, y);
    }
    void putchar(int c)
    {
        switch(c)
        {
            case '\t':
            {
                for(int i = 0; i < 4; ++i) putchar(' ');
                break;
            }
            case '\r':
            {
                cur_x = 0;
                break;
            }
            case '\b':
            {
                if(cur_y == 0 && cur_x == 0) 
                {
                    break;
                }
                else if (cur_y == 0 && cur_x != 0)
                {
                    cur_y = 80 - 1;
                    cur_x--;
                    putchar_worker(' ', MAKE_COLOR(VGA_BLACK, VGA_WHITE) , cur_x, cur_y);
                    break;
                }
                else
                {
                    cur_y--;
                    putchar_worker(' ', MAKE_COLOR(VGA_BLACK, VGA_WHITE) , cur_x, cur_y);
                    break;
                }
            }
            case '\n':
            {
                cur_x++;
                cur_y = 0;
                break;
            }
            default:
            {
                putchar_worker(c, color, cur_x, cur_y);
                cur_x = (cur_y == 79) ? cur_x + 1 : cur_x;
                cur_y = (cur_y + 1) % 80;
                break;
            }
        }
        move_cursor(cur_x, cur_y);
    }
};
#endif