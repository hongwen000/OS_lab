#ifndef _STRING_H
#define _STRING_H

#include "defines.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

#ifndef __cplusplus
extern "C" {
#endif
 
int memcmp(const void* _s1, const void* _s2, size_t n);
void* memcpy(void* _dst, const void* _src, size_t n);
void* memmove(void* _dst, const void* _src, size_t n);
void* memset(void* _dst, int c, size_t n);
size_t strlen(const char* _str);
char *strcpy( char* _dst, const char* _src);
int strcmp( const char* _s1, const char* _s2);
//void itoa(char* buffer, int num, int base);
void utoa(char* buffer, unsigned int num, int base);
void reverse(char* buffer);
template<typename T>
void itoa(char* buffer, T num, int base, bool captial = false)
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
    while(true)
    {
        mod = num % base;
        num /= base;
        *(p++) = captial ? num_char_upper[mod] : num_char_lower[mod];
        if(num == 0) break;
    }
    *p = '\0';
    reverse(buffer);
}
#ifndef __cplusplus
}
#endif

#ifdef _HHOS_LIBC_TEST
}
#endif

#endif