#ifndef _CTYPE_H
#define _CTYPE_H

#include "../include/defines.h"

#include "../basic_lib/sys_lib.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

#ifndef __cplusplus
extern "C" {
#endif
 
int isspace( int ch );

#ifndef __cplusplus
}
#endif

#ifdef _HHOS_LIBC_TEST
}
#endif

#endif