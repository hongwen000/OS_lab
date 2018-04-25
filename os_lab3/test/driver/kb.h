#ifndef __KB_H
#define __KB_H
#define KBSTATP     0x64
#define KBS_DIB     0x01
#define KBDATAP     0x60
#define KB_DATA     0x60
#define KB_STAT     0x64
#define KB_CMD      0x64
#define TTY_EOF     254
#define TTY_INT     255

/* output buffer full */
#define KB_STAT_OBF     0x01

#define SHIFT       (1<<0)
#define CTRL        (1<<1)
#define ALT         (1<<2)

#define CAPSLOCK        (1<<3)
#define NUMLOCK         (1<<4)
#define SCROLLLOCK      (1<<5)
#define E0ESC       (1<<6)

#define KB_IS_RELEASE(sc)   (sc & 0x80) 
#define KB_IS_ESCAPE(sc)    (sc == 0xe0)
#define KB_BUF_LEN  32

// Special keycodes
#define KEY_HOME        0xE0
#define KEY_END         0xE1
#define KEY_UP          0xE2
#define KEY_DN          0xE3
#define KEY_LF          0xE4
#define KEY_RT          0xE5
#define KEY_PGUP        0xE6
#define KEY_PGDN        0xE7
#define KEY_INS         0xE8
#define KEY_DEL         0xE9

// C('A') == Control-A
#define C(x) (x - '@')

#ifdef __cplusplus
extern "C"{
#endif
unsigned char kb_buf_out();

#ifdef __cplusplus
}
#endif
#endif

