#include "shared/memory.h"

#include <stdlib.h>

void fl_mem_free(fl_unknown *memory)
{
    free(memory);
}

fl_unknown *fl_mem_alloc(fl_size size)
{
    return malloc(size);
}

fl_unknown *fl_mem_alloc_arr(fl_size size, fl_size count)
{
    return malloc(size * count);
}
