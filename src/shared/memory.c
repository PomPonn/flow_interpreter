#include "shared/memory.h"

#include <stdlib.h>
#include <string.h>

fl_void fl_mem_free(fl_unknown *memory)
{
    free(memory);
}

fl_unknown *fl_mem_alloc(fl_size size)
{
    return malloc(size);
}

fl_unknown *fl_mem_realloc(fl_unknown *mem, fl_size new_size)
{
    return realloc(mem, new_size);
}

fl_unknown *fl_mem_alloc_arr(fl_size elem_size, fl_size count)
{
    return malloc(elem_size * count);
}

fl_void fl_mem_copy(const fl_unknown *src, fl_unknown *dest, fl_size size)
{
    memcpy(dest, src, size);
}

fl_void fl_mem_zero(fl_unknown *memory, fl_size count)
{
    memset(memory, 0, count);
}