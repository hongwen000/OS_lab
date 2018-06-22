#include "kb.h"

#define NO 0

typedef unsigned long size_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
#define KB_DBG
#ifdef KB_DBG
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
static inline void sys_outw(uint16_t port, uint16_t data)
{
    asm volatile ("outw %0, %1"
    :
    :"a"(data), "Nd"(port));
}
#define bochs_break()       \
        sys_outw(0x8A00,0x8A00);    \
        sys_outw(0x8A00,0x08AE0);
static inline void sys_dbg_bochs_putc(char c){
//    sys_outb(0xe9, (uint8_t)c);
}
static inline int debug_putchar( int ch )
{
    //echo -e "\e[1;31m This is red text! \e[0m"
    sys_dbg_bochs_putc('\e');
    sys_dbg_bochs_putc('[');
    sys_dbg_bochs_putc('1');
    sys_dbg_bochs_putc(';');
    sys_dbg_bochs_putc('3');
    sys_dbg_bochs_putc('1');
    sys_dbg_bochs_putc('m');
    sys_dbg_bochs_putc(ch);
    sys_dbg_bochs_putc('\e');
    sys_dbg_bochs_putc('[');
    sys_dbg_bochs_putc('0');
    sys_dbg_bochs_putc('m');
    return ch;
}
static inline size_t strlen(const char* _str)
{
    const char* str = _str;
    size_t len = 0;
    while(_str && *(_str++))
    {
        len++;
    }
    return len;
}
static inline void* memcpy(void* _dst, const void* _src, size_t n)
{
    unsigned char* dst = (unsigned char*)_dst;
    const unsigned char* src = (const unsigned char*)_src;
    while (n--)
    {
        *(dst++) = *(src++);
    }
    return _dst;
}
static inline char *strcpy(char* _dst, const char* _src)
{
    size_t len = strlen(_src);
    memcpy((void*)(_dst), (const void *)(_src), len);
    _dst[len] = '\0';
    return _dst;
}
static inline int debug_puts(const char* string)
{
    size_t len = strlen(string);
    for(size_t i = 0; i < len; ++i)
    {
        debug_putchar(*(string + i));
    }
    return 0;
}
typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start(ap, last)

#define va_arg(ap, type) __builtin_va_arg(ap, type)

#define va_end(ap) __builtin_va_end(ap)
static inline void reverse(char* buffer)
{
    char ch;
    size_t len = strlen(buffer);
    for(size_t i = 0; i < len / 2; ++i)
    {
        ch = *(buffer + i);
        *(buffer + i) = *(buffer + len - 1 - i);
        *(buffer + len - 1 - i)  = ch;
    }
}
static inline void itoa(char* buffer, int num, int base, int captial)
{
    const char* num_char_upper = "0123456789ABCDEF";
    const char* num_char_lower = "0123456789abcdef";
    if(num < 0)
    {
        *(buffer++) = '-';
        num = -num;
    }
    char *p = buffer;
    int mod = 0;
    while(1)
    {
        mod = num % base;
        num /= base;
        *(p++) = captial ? num_char_upper[mod] : num_char_lower[mod];
        if(num == 0) break;
    }
    *p = '\0';
    reverse(buffer);
}
static inline int debug_vsprintf( char* buffer, const char* format, va_list vlist )
{
    size_t len_fmt = strlen(format);
    const char* p_fmt = format;
    char* p_buf = buffer;
    while(len_fmt--)
    {
        if(*p_fmt != '%')
        {
            *(p_buf++) = *(p_fmt++);    //非控制字符
            continue;
        }
        else
        {
            //此时p_fmt = '%'
            char specifier = *(++p_fmt); //%后面一个字符是specifier
            ++p_fmt;                     //记录specifier并跳过
            switch (specifier)
            {
                case 'c':
                {
                    /*
                    依据C语言标准
                    If you pass an integral value with conversion rank
                    smaller than that of int (e. g. char, bool or short)
                     to a function taking a variable number of arguments,
                     it will be converted to int
                    */
                    *(p_buf++) = va_arg(vlist, int);
                    break;
                }
                case 's':
                {
                    const char * str = va_arg(vlist, char*);
                    strcpy(p_buf, str);
                    p_buf += strlen(str);               //p应当永远指向有意义的字符后面一位
                    break;
                }
                case 'd':
                case 'i':
                {
                    int num = va_arg(vlist, int);
                    itoa(p_buf, num, 10, 0);
                    p_buf += strlen(p_buf);             //同理
                    break;
                }
                case 'o':
                {
                    unsigned int num = va_arg(vlist, int);
                    itoa(p_buf, num, 8, 0);
                    p_buf += strlen(p_buf);             //同理
                    break;
                }
                case 'x':
                {
                    unsigned int num = va_arg(vlist, int);
                    itoa(p_buf, num, 16, 0);
                    p_buf += strlen(p_buf);             //同理
                    break;
                }
                case 'X':
                {
                    unsigned int num = va_arg(vlist, int);
                    itoa(p_buf, num, 16, 1);
                    p_buf += strlen(p_buf);             //同理
                    break;
                }
                case 'u':
                {
                    unsigned int num = va_arg(vlist, int);
                    itoa(p_buf, num, 10, 0);
                    p_buf += strlen(p_buf);             //同理
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
    *p_buf = '\0';
    return p_buf - buffer;
}

static inline int debug_sprintf( char *buffer, const char *format, ... )
{
    int ret;
    va_list va;
    va_start(va, format);
    ret = debug_vsprintf(buffer, format, va);
    va_end(va);
    return ret;
}
static char debug_printbuf[80*25];
//TODO 关于这个返回值
//static inline int debug_printf( const char* format, ... )
//{
//    int ret;
//    va_list va;
//    va_start(va, format);
//    ret = debug_vsprintf(debug_printbuf, format, va);
//    va_end(va);
//    debug_puts(debug_printbuf);
//    return ret;
//}
#endif

unsigned char kb_buf[KB_BUF_LEN];

int buf_pos = 0;

int kb_buf_in(unsigned char ch)
{
    kb_buf[buf_pos] = ch;
    __sync_add_and_fetch (&buf_pos, 1);
    return 0;
}
unsigned char kb_buf_out()
{
    unsigned char ch = 0;
    while(ch == 0)
    {
        int val = __sync_add_and_fetch(&buf_pos, 0);
        asm volatile("sti\n\t");
        while(val == 0){
            val = __sync_add_and_fetch(&buf_pos, 0);
        }
        asm volatile("cli\n\t");
        ch = kb_buf[val-1];
        __sync_fetch_and_sub(&buf_pos, 1);
    }
    return ch;
}

uint32_t kb_mode = 0;

unsigned char combine_key[256] =
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

static unsigned int ctrl_shift_status = 0;
void kb_init()
{
    ctrl_shift_status = 0;
}
unsigned char *charcode[4] = {
  kb_map, kb_shift_map, kb_ctrl_map, kb_ctrl_map
};
void kb_handler() {
  unsigned int st, data, c;

  st = sys_inb(0x64);
  if((st & 0x01) == 0)
    return;
  data = sys_inb(0x60);
//  debug_printf("kb_handler: scancode %u\n", data);
  if(data == 0xE0){
    //如果是第一次发送过来的扩展键标志，记录并返回
    ctrl_shift_status |= E0ESC;
//      debug_printf("kb_handler: .1 ctrl_shift_status %u\n", ctrl_shift_status);
    return;
  }
  else if(data & 0x80){
    //可能是组合键的断码，需要特别考虑并清除该组合键状态
    data = (ctrl_shift_status & E0ESC ? data : data & 0x7F);
    ctrl_shift_status &= ~(combine_key[data] | E0ESC);
//      debug_printf("kb_handler: .2 ctrl_shift_status %u\n", ctrl_shift_status);
    return;
  }
  else if(ctrl_shift_status & E0ESC){
    //进入这里说明上次按键是扩展键标志
    data |= 0x80;
    ctrl_shift_status &= ~E0ESC;//扩展键低位读取到结束，清除扩展位标志
//      debug_printf("kb_handler: .3 ctrl_shift_status %u\n", ctrl_shift_status);
  }
  //添加组合键状态
  ctrl_shift_status |= combine_key[data];
//    debug_printf("kb_handler: .4 ctrl_shift_status %u\n", ctrl_shift_status);
  //决定状态键状态(使用异或来回切换)
  ctrl_shift_status ^= togglecode[data];
//    debug_printf("kb_handler: .5 ctrl_shift_status %u\n", ctrl_shift_status);
  c = charcode[ctrl_shift_status & (CTRL | SHIFT)][data];
//  debug_printf("kb_handler: charcode[%u] is at 0x%x\n", ctrl_shift_status & (CTRL | SHIFT),&charcode[ctrl_shift_status & (CTRL | SHIFT)]);
  if(ctrl_shift_status & CAPSLOCK){
    if('a' <= c && c <= 'z')
      c += 'A' - 'a';
    else if('A' <= c && c <= 'Z')
      c += 'a' - 'A';
  }
//  debug_printf("kb_handler: got char %c (%d)", c, (int)c);
  kb_buf_in(c);
}
