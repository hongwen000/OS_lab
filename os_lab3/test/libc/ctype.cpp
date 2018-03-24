#include "ctype.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

int isspace( int ch )
{
    return (ch == 9 || ch == 32 || (ch >= 10 && ch <= 13));
}

#ifdef _HHOS_LIBC_TEST
}
#endif