#ifndef _STDLIB_H
#define _STDLIB_H

#include "../include/defines.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

#ifndef __cplusplus
extern "C" {
#endif
 
int abs( int n );
long labs( long n );
struct div_t { int quot; int rem; };
struct ldiv_t { long quot; long rem; };
div_t div( int x, int y );
ldiv_t ldiv( long x, long y );
int atoi(const char* s);
long atol(const char* s);
long strtol( const char *str, char **str_end, int base );
char* strtol_get_end( const char *str, char **str_end, int base );
int rand();
void srand(unsigned int seed);
#ifndef __cplusplus
}
#endif

#ifdef _HHOS_LIBC_TEST
}
#endif

#endif
