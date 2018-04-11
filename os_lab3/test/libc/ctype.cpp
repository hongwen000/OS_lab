#include "./ctype.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

int isspace( int ch )
{
    return (ch == 9 || ch == 32 || (ch >= 10 && ch <= 13));
}

int isalnum(int ch)
{
    return isdigit(ch) || isalpha(ch);
}

int isdigit(int ch)
{
    return (ch >= 48 && ch <= 57);
}

int isalpha(int ch)
{
    return  isupper(ch)||islower(ch);
}
int isupper(int ch)
{
    return (ch >= 65 && ch <= 90);
}

int islower(int ch)
{
    return (ch >= 97 && ch <= 122);
}

int iscntrl(int ch)
{
    return (ch >= 0 && ch <=31) || ch == 127;
}

int isgraph(int ch)
{
    return (ch >= 33 && ch <= 126);
}

int isprint(int ch)
{
    return (ch >= 32 && ch <= 126);
}

int ispunct(int ch)
{
    return (ch >= 33 && ch <=47) || (ch >= 58 && ch <=64) 
            || (ch >= 91 && ch <= 96) || (ch >= 123 && ch <= 126);
}

int isxdigit(int ch)
{
    return isdigit(ch) || (ch >= 65 && ch <= 70) || (ch >= 97 && ch <= 102);
}
int tolower(int ch)
{
    return ch + 0x20;
}
int toupper(int ch)
{
    return ch - 0x20;
}

#ifdef _HHOS_LIBC_TEST
}
#endif
