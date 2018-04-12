// #define __LOG_ON 1

/* kb.c 
 * This file is modified form Bram's Kernel Development Tutorial
 * enable keyboard and retive message by IRQ
 * October is bad :(
 * http://bbs.chinaunix.net/thread-1999379-1-1.html
 * borrow code form: https://github.com/Fleurer/fleurix/blob/master/src/chr/keybd.c
 */
// std
#include "kb.h"
#define NO 0
typedef unsigned long size_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

static inline uint8_t sys_inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

static inline void sys_outb(uint16_t port, uint8_t data)
{
    asm volatile ( "outb %0, %1"
                   : 
                   : "a"(data), "Nd"(port) );
}

static inline char sys_get_scancode()
{
    while (!(sys_inb(0x64) & 1));
    return sys_inb(0x60);
}

unsigned char kb_buf[KB_BUF_LEN];

int buf_pos = 0;

extern void sys_bios_print_string(const char* str, unsigned int len, int color, int pos);

static char *itoa(unsigned char val, char *buf, unsigned radix)
{
    char   *p;
    char   *firstdig;
    char   temp;
    unsigned   digval;
    p = buf;
    if(val <0)
    {
        *p++ = '-';
        val = (unsigned long)(-(long)val);
    }
    firstdig = p;
    do{
        digval = (unsigned)(val % radix);
        val /= radix;

        if  (digval > 9)
            *p++ = (char)(digval - 10 + 'a');
        else
            *p++ = (char)(digval + '0');
    }while(val > 0);

    *p-- = '\0';
    do{
        temp = *p;
        *p = *firstdig;
        *firstdig = temp;
        --p;
        ++firstdig;
    }while(firstdig < p);
    return buf;
}
static inline void sys_bios_print_int(unsigned char num, int color, int pos)
{
    unsigned char num_ = num;
    int len = 1;
    while(num_ /= 10) len++;
    char arr[len + 1];
    itoa(num, arr, 10);
    sys_bios_print_string(arr, len, color, pos);
}

#define MAKE_COLOR(BACKG, FOREG) \
    (((BACKG << 4) | (FOREG & 0x0F)))

#define MAKE_POS(ROW, COLUMN) \
    ((ROW << 8) | (COLUMN & 0xFF))

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

int kb_buf_in(unsigned char ch)
{
    kb_buf[buf_pos] = ch;
    __sync_add_and_fetch (&buf_pos, 1);
    return 0;
}
unsigned char kb_buf_out()
{
    int val = __sync_add_and_fetch(&buf_pos, 0);
    asm volatile("sti\n\t");
    while(val == 0){
        val = __sync_add_and_fetch(&buf_pos, 0);
    }
    asm volatile("cli\n\t");
    unsigned char ch = kb_buf[val-1];
    __sync_fetch_and_sub(&buf_pos, 1);
    return ch;
}

uint32_t kb_mode = 0;

unsigned char shiftcode[256] =
{
  [0x1D] = CTRL,
  [0x2A] = SHIFT,
  [0x36] = SHIFT,
  [0x38] = ALT,
  [0x9D] = CTRL,
  [0xB8] = ALT
};

unsigned char togglecode[256] =
{
  [0x3A] = CAPSLOCK,
  [0x45] = NUMLOCK,
  [0x46] = SCROLLLOCK
};

unsigned char kb_map[256] =
{
  NO,   0x1B, '1',  '2',  '3',  '4',  '5',  '6',  // 0x00
  '7',  '8',  '9',  '0',  '-',  '=',  '\b', '\t',
  'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  // 0x10
  'o',  'p',  '[',  ']',  '\n', NO,   'a',  's',
  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  // 0x20
  '\'', '`',  NO,   '\\', 'z',  'x',  'c',  'v',
  'b',  'n',  'm',  ',',  '.',  '/',  NO,   '*',  // 0x30
  NO,   ' ',  NO,   NO,   NO,   NO,   NO,   NO,
  NO,   NO,   NO,   NO,   NO,   NO,   NO,   '7',  // 0x40
  '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',  NO,   NO,   NO,   NO,   // 0x50
  [0x9C] = '\n',      // KP_Enter
  [0xB5] = '/',       // KP_Div
  [0xC8] = KEY_UP,    [0xD0] = KEY_DN,
  [0xC9] = KEY_PGUP,  [0xD1] = KEY_PGDN,
  [0xCB] = KEY_LF,    [0xCD] = KEY_RT,
  [0x97] = KEY_HOME,  [0xCF] = KEY_END,
  [0xD2] = KEY_INS,   [0xD3] = KEY_DEL
};

unsigned char kb_shift_map[256] =
{
  NO,   033,  '!',  '@',  '#',  '$',  '%',  '^',  // 0x00
  '&',  '*',  '(',  ')',  '_',  '+',  '\b', '\t',
  'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',  // 0x10
  'O',  'P',  '{',  '}',  '\n', NO,   'A',  'S',
  'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',  // 0x20
  '"',  '~',  NO,   '|',  'Z',  'X',  'C',  'V',
  'B',  'N',  'M',  '<',  '>',  '?',  NO,   '*',  // 0x30
  NO,   ' ',  NO,   NO,   NO,   NO,   NO,   NO,
  NO,   NO,   NO,   NO,   NO,   NO,   NO,   '7',  // 0x40
  '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',  NO,   NO,   NO,   NO,   // 0x50
  [0x9C] = '\n',      // KP_Enter
  [0xB5] = '/',       // KP_Div
  [0xC8] = KEY_UP,    [0xD0] = KEY_DN,
  [0xC9] = KEY_PGUP,  [0xD1] = KEY_PGDN,
  [0xCB] = KEY_LF,    [0xCD] = KEY_RT,
  [0x97] = KEY_HOME,  [0xCF] = KEY_END,
  [0xD2] = KEY_INS,   [0xD3] = KEY_DEL
};

unsigned char kb_ctrl_map[256] =
{
  NO,      NO,      NO,      NO,      NO,      NO,      NO,      NO,
  NO,      NO,      NO,      NO,      NO,      NO,      NO,      NO,
  C('Q'),  C('W'),  C('E'),  C('R'),  C('T'),  C('Y'),  C('U'),  C('I'),
  C('O'),  C('P'),  NO,      NO,      '\r',    NO,      C('A'),  C('S'),
  C('D'),  C('F'),  C('G'),  C('H'),  C('J'),  C('K'),  C('L'),  NO,
  NO,      NO,      NO,      C('\\'), C('Z'),  C('X'),  C('C'),  C('V'),
  C('B'),  C('N'),  C('M'),  NO,      NO,      C('/'),  NO,      NO,
  [0x9C] = '\r',      // KP_Enter
  [0xB5] = C('/'),    // KP_Div
  [0xC8] = KEY_UP,    [0xD0] = KEY_DN,
  [0xC9] = KEY_PGUP,  [0xD1] = KEY_PGDN,
  [0xCB] = KEY_LF,    [0xCD] = KEY_RT,
  [0x97] = KEY_HOME,  [0xCF] = KEY_END,
  [0xD2] = KEY_INS,   [0xD3] = KEY_DEL
};



unsigned char dect_mode(char sc){
    char ch = sc & 0x7f;

    if (kb_mode & E0ESC) { 
        switch (ch) {
            case 0x1D: return CTRL;
        }
    }
    else {
        switch(ch) {
            case 0x2A:
            case 0x36: return SHIFT;
            case 0x1D: return CTRL;
        }
    }
    return 0;
}

//void kb_handler(){
//    unsigned char sc, m;
//    unsigned char ch;
//
//    //sc = sys_get_scancode();
//    if((sys_inb(0x64) & KB_STAT_OBF) == 0){
//        return;
//    }
//    sc = sys_inb(0x60);
//
//    if (KB_IS_ESCAPE(sc)){
//        kb_mode |= E0ESC;
//        return;
//    } 
//    
//    if ((m = dect_mode(sc))){
//        if (KB_IS_RELEASE(sc)){
//            kb_mode &= ~m;
//        } else {
//            kb_mode |= m;
//        }
//        return;
//    }
//
//    if (kb_mode & SHIFT){
//        ch = kb_shift_map[sc & 0x7F];
//    } else if (kb_mode & CTRL) {
//        ch = CTRLmap[sc & 0x7F];
//    } else {
//        ch = kb_map[sc & 0x7F];
//    }
//
//    if (KB_IS_RELEASE(sc)){
//        kb_mode &= ~E0ESC;
//    } 
//    else if (ch != 0) {
//        if (kb_buf_in(ch) < 0){
//        }
//    }
//}
//
unsigned int ctrl_shift_status;
unsigned char *charcode[4] = {
  kb_map, kb_shift_map, kb_ctrl_map, kb_ctrl_map
};
void kb_handler() {
  unsigned int st, data, c;

  st = sys_inb(KBSTATP);
  if((st & KBS_DIB) == 0)
    return;
  data = sys_inb(KBDATAP);

  if(data == 0xE0){
    ctrl_shift_status |= E0ESC;
    return;
  } else if(data & 0x80){
    // Key released
    data = (ctrl_shift_status & E0ESC ? data : data & 0x7F);
    ctrl_shift_status &= ~(shiftcode[data] | E0ESC);
    return;
  } else if(ctrl_shift_status & E0ESC){
    // Last character was an E0 escape; or with 0x80
    data |= 0x80;
    ctrl_shift_status &= ~E0ESC;
  }

  ctrl_shift_status |= shiftcode[data];
  ctrl_shift_status ^= togglecode[data];
  c = charcode[ctrl_shift_status & (CTRL | SHIFT)][data];
  if(ctrl_shift_status & CAPSLOCK){
    if('a' <= c && c <= 'z')
      c += 'A' - 'a';
    else if('A' <= c && c <= 'Z')
      c += 'a' - 'A';
  }
  kb_buf_in(c);
}
