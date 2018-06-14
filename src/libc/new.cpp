//
// Created by 李新锐 on 14/06/2018.
//
#include "new.h"
void *operator new(size_t size)
{
    return malloc(size);
}

void *operator new[](size_t size)
{
    return malloc(size);
}

void operator delete(void *p)
{
    free(p);
}

void operator delete[](void *p)
{
    free(p);
}

void operator delete(void * ptr, size_t)
{
	operator delete(ptr);
}
void _Unwind_Resume (_Unwind_Exception *){}
