#ifndef _BASIC_DEFINE_
#define _BASIC_DEFINE_

#ifndef _CODE16GCC_H_
#define _CODE16GCC_H_
__asm__(".code16gcc\n");
#endif

enum VGA_COLOR{
    BLACK           =   0x00,    
    BLUE            =   0x01,    
    GREEN           =   0x02,    
    CYAN            =   0x03,    
    RED             =   0x04,    
    MAGENTA         =   0x05,    
    BROWN           =   0x06,    
    WHITE           =   0x07,     
    GRAY			=   0x08,
    BRIGHT_BLUE		=   0x09,
    BRIGHT_GREEN	=   0x0A,
    BRIGHT_CYAN		=   0x0B,
    BRIGHT_RED		=   0x0C,
    BRIGHT_MAGENTA	=   0x0D,
    YELLOW			=   0x0E,
    BRIGHT_WHITE	=   0x0F
};

#define MAKE_COLOR(BACKG, FOREG) \
    (((BACKG << 4) | (FOREG & 0x0F)))

#define MAKE_POS(ROW, COLUMN) \
    ((ROW << 8) | (COLUMN & 0xFF))

#endif
