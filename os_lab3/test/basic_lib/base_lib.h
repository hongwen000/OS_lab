#include "defines.h"

#ifdef __cplusplus
extern "C"{
#endif 

    void sys_print_string(const char* str, unsigned int len, int color, int pos);
    void sys_print_int(int num, int color, int pos);
    void sys_clear_screen();

#ifdef __cplusplus
}
#endif
