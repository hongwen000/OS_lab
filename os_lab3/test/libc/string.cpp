#include "string.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

int memcmp(const void * _s1, const void* _s2, size_t n)
{
    auto s1 = reinterpret_cast<const unsigned char*>(_s1);
    auto s2 = reinterpret_cast<const unsigned char*>(_s2);
    while(n--)
    {
        if(*s1 != *s2)          //**不能**让!=时跳出然后返回*s1 - *s2
        {                       //因为那样在实际上==的时候，会比较错误
            return *s1 - *s2;
        }
        ++s1;
        ++s2;
    }
    return 0;
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
        *(dst++) = static_cast<unsigned char>(c);
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
char *strcpy(char* _dst, const char* _src)
{
    auto len = strlen(_src);
    memcpy(reinterpret_cast<void*>(_dst), reinterpret_cast<const void *>(_src), len);
    _dst[len] = '\0';
    return _dst;
}
int strcmp(const char* _s1, const char* _s2)
{
    auto len1 = strlen(_s1);
    auto len2 = strlen(_s2);
    auto len = len1 < len2 ? len1 : len2;
    auto ret = memcmp(reinterpret_cast<const void*>(_s1), reinterpret_cast<const void *>(_s2), len);
    if(ret == 0)
    {
        if(len1 == len2)
        {
            return 0;
        }
        else if (len == len1)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    return ret;
}

void reverse(char* buffer)
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

//void utoa(char* buffer, unsigned int num, int base)
//{
//    char *p = buffer;
//    int mod = 0;
//    while(true)
//    {
//        mod = num % base;
//        num /= base;
//        *(p++) = num_char[mod];
//        if(num == 0) break;
//    }
//    *p = '\0';
//    reverse(buffer);
//}
// template<typename T>
// void itoa(char* buffer, T num, int base)
// {
//     if(num < 0)
//     {
//         *(buffer++) = '-';
//         num = -num;
//     }
//     char *p = buffer;
//     int mod = 0;
//     while(true)
//     {
//         mod = num % base;
//         num /= base;
//         *(p++) = num_char[mod];
//         if(num == 0) break;
//     }
//     *p = '\0';
//     reverse(buffer);
// }

#ifdef _HHOS_LIBC_TEST
}
#endif
