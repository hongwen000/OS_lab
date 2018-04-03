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
    return ((ch >= 48 && ch <= 57) || (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122));
}

int isdigit(int ch)
{
    return (ch >= 48 && ch <= 57);
}
#ifdef _HHOS_LIBC_TEST
}
#endif