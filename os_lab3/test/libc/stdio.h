#ifndef _STRING_H
#define _STRING_H

#include "defines.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

#ifdef __cplusplus
extern "C" {
#endif
 
int puts(const char* string);
int printf( const char* format, ... );
int putchar( int ch );
int scanf( const char* format, ... );
int getchar(void);
char *gets( char *str );
int rand();
void srand( unsigned seed );

#ifdef __cplusplus
}
#endif

#ifdef _HHOS_LIBC_TEST
}
#endif

#endif