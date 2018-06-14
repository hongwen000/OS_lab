#include "../../include/defines.h"
#include "../../kernel_lib/ram.h"
#include "../../kernel_lib/page.h"
#include "../mutex.h"
#include "hhos.h"


#ifdef __cplusplus
extern "C" {
#endif
static int mut;
static bool mut_inited = false;
int liballoc_lock()
{
    if(!mut_inited)
    {
        mut = mutex_get();
        mut_inited = true;
    }
    mutex_lock(mut);
    return 0;
}

int liballoc_unlock()
{
    mutex_unlock(mut);
    return 0;
}

void* liballoc_alloc(size_t pages)
{
    unsigned int size = pages * PAGE_SIZE;
    char *p2 = (char*) sbrk(size);
    if ( p2 == nullptr) return nullptr;

    return p2;
}

int liballoc_free(void* ptr, size_t pages)
{
    return munmap( ptr, pages * PAGE_SIZE );
}


#ifdef __cplusplus
}
#endif




