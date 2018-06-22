#ifndef _STDIO_H
#define _STDIO_H

#include "../include/defines.h"
#include "sys/hhos.h"

#ifndef _HHOS_LIBC_TEST
#define EOF -1
#endif

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

#ifndef __cplusplus
extern "C" {
#endif
    using fpos_t = int32_t;
    struct __sbuf {
        unsigned char	*_base;
        int		_size;
    };
    struct __sFILEX;
    typedef	struct __sFILE {
        unsigned char *_p;	/* current position in (some) buffer */
        int	_r;		/* read space left for getc() */
        int	_w;		/* write space left for putc() */
        short	_flags;		/* flags, below; this FILE is free if 0 */
        short	_file;		/* fileno, if Unix descriptor, else -1 */
        struct	__sbuf _bf;	/* the buffer (at least 1 byte, if !NULL) */
        int	_lbfsize;	/* 0 or -_bf._size, for inline putc */

        /* operations */
        void	*_cookie;	/* cookie passed to io functions */
        int	(* _close)(void *);
        int	(* _read) (void *, char *, int);
        fpos_t	(* _seek) (void *, fpos_t, int);
        int	(* _write)(void *, const char *, int);

        /* separate buffer for long sequences of ungetc() */
        struct	__sbuf _ub;	/* ungetc buffer */
        struct __sFILEX *_extra; /* additions to FILE to not break ABI */
        int	_ur;		/* saved _r when _r is counting ungetc data */

        /* tricks to meet minimum requirements even when malloc() fails */
        unsigned char _ubuf[3];	/* guarantee an ungetc() buffer */
        unsigned char _nbuf[1];	/* guarantee a getc() buffer */

        /* separate buffer for fgetln() when line crosses buffer boundary */
        struct	__sbuf _lb;	/* buffer for fgetln() */

        /* Unix stdio files get aligned to block boundaries on fseek() */
        int	_blksize;	/* stat.st_blksize (may be != _bf._size) */
        fpos_t	_offset;	/* current lseek offset (see WARNING) */
    } FILE;

int putchar( int ch );
int puts(const char* string);
int vsprintf( char* buffer, const char* format, va_list vlist );
int sprintf( char *buffer, const char *format, ... );
int printf( const char* format, ... );

int getchar(void);
char *gets( char *str );
int vsscanf( const char* buffer, const char* format, va_list vlist );
int sscanf( const char* buffer, const char* format, ... );
int scanf( const char* format, ... );

int scanf( const char* format, ... );

#ifndef __cplusplus
}
#endif

#ifdef _HHOS_LIBC_TEST
}
#endif

#endif
