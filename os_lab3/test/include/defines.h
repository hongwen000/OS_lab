#ifndef _HHOS_BASIC_DEFINE_
#define _HHOS_BASIC_DEFINE_



#ifndef _HHOS_LIBC_TEST

// #ifndef _CODE16GCC_H_
// #define _CODE16GCC_H_
// __asm__(".code16gcc\n");
// #endif

typedef __builtin_va_list va_list;

#define va_start(ap, last) __builtin_va_start(ap, last)

#define va_arg(ap, type) __builtin_va_arg(ap, type)

#define va_end(ap) __builtin_va_end(ap)
#else
#include <stdarg.h>
#endif

#define USE_BOCHS_DEBUG_OUTPUT

using size_t = unsigned long;
using uint8_t = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;


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
