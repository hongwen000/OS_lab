#include "string.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

int memcmp(const void * _s1, const void* _s2, size_t n)
{
    auto s1 = reinterpret_cast<const unsigned char*>(_s1);
    auto s2 = reinterpret_cast<const unsigned char*>(_s2);
    while(n-- && (*(s1++) == *(s2++)));
    return *s1 - *s2;
}

void* memcpy(void* _dst, const void* _src, size_t n)
{
    auto dst = reinterpret_cast<unsigned char*>(_dst);
    auto src = reinterpret_cast<const unsigned char*>(_src);
    while (n--)
    {
        *(dst++) = *(src++);
    }
    return _dst;
}
void* memmove(void* _dst, const void* _src, size_t n)
{
    auto dst = reinterpret_cast<unsigned char*>(_dst);
    auto src = reinterpret_cast<const unsigned char*>(_src);
    if(dst < src)
    {
        while(n--)
        {
            *(dst++) = *(src++);
        }
    }
    else if(dst > src)
    {
        src += n;
        dst += n;
        while(n--)
        {
            *(--dst) = *(--src);
        }
    }
    return _dst;
}
void* memset(void* _dst, int c, size_t n)
{
    auto dst = reinterpret_cast<unsigned char*>(_dst);
    while(n--)
    {
        *dst = static_cast<unsigned char>(c);
    }
    return _dst;
}
size_t strlen(const char* _str)
{
    auto str = _str;
    size_t len = 0;
    while(*(_str++))
    {
        len++;
    }
    return len;
}

#ifdef _HHOS_LIBC_TEST
}
#endif