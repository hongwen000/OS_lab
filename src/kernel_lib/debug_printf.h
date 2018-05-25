//
// Created by 李新锐 on 23/04/2018.
//

#ifndef HHOS_DEBUG_PRINTF_H
#define HHOS_DEBUG_PRINTF_H

#include "../include/defines.h"
#include "../kernel/tty.h"

extern "C" {

int debug_printf( const char* format, ... );
int debug_puts(const char* string);
int tty_debug_printf(tty& dbg_tty, const char* format, ... );
void real_world_cls();
};
#endif //HHOS_DEBUG_PRINTF_H
