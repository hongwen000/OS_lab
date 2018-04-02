#ifndef _STDLIB_H
#define _STDLIB_H

#include "../include/defines.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

#ifndef __cplusplus
extern "C" {
#endif
 
long strtol( const char *str, char **str_end, int base );

#ifndef __cplusplus
}
#endif

#ifdef _HHOS_LIBC_TEST
}
#endif

#endif