#ifndef _BASIC_DEFINE_
#define _BASIC_DEFINE_

#ifndef _HHOS_LIBC_TEST

#ifndef _CODE16GCC_H_
#define _CODE16GCC_H_
__asm__(".code16gcc\n");
#endif

#endif

#include <stddef.h>

#include <stdint.h>

#include <stdalign.h>

#include <limits.h>

#include <stdarg.h>

enum VGA_COLOR{
    VGA_BLACK           =   0x00,    
    VGA_BLUE            =   0x01,    
    VGA_GREEN           =   0x02,    
    VGA_CYAN            =   0x03,    
    VGA_RED             =   0x04,    
    VGA_MAGENTA         =   0x05,    
    VGA_BROWN           =   0x06,    
    VGA_WHITE           =   0x07,     
    VGA_GRAY			=   0x08,
    VGA_BRIGHT_BLUE		=   0x09,
    VGA_BRIGHT_GREEN	=   0x0A,
    VGA_BRIGHT_CYAN		=   0x0B,
    VGA_BRIGHT_RED		=   0x0C,
    VGA_BRIGHT_MAGENTA	=   0x0D,
    VGA_YELLOW			=   0x0E,
    VGA_BRIGHT_WHITE	=   0x0F
};

#define MAKE_COLOR(BACKG, FOREG) \
    (((BACKG << 4) | (FOREG & 0x0F)))

#define MAKE_POS(ROW, COLUMN) \
    ((ROW << 8) | (COLUMN & 0xFF))

#define KEY_DOWN_PRESSED    0x50

#define __HHOS_libc

#endif
