#include "../include/defines.h"
#include "./stdio.h"

#ifdef NDEBUG
#define hhos_assert(condition) ((void)0)
#else
#define hhos_assert(condition) if(!(condition)) \
     printf("HHOS Assertion failed: %s, file %s, line %i\n", \
        #condition, __FILE__, __LINE__)
#endif

