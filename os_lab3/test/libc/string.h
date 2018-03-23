#ifndef _STRING_H
#define _STRING_H

#include "defines.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

#ifdef __cplusplus
extern "C" {
#endif
 
int memcmp(const void*, const void*, size_t);
void* memcpy(void*, const void*, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);
 
#ifdef __cplusplus
}
#endif

#ifdef _HHOS_LIBC_TEST
}
#endif

#endif