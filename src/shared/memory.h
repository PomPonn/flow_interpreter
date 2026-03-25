#pragma once

#include "shared/types.h"

fl_void fl_mem_free(fl_unknown *memory);

fl_unknown *fl_mem_alloc(fl_size size);

fl_unknown *fl_mem_alloc_arr(fl_size elem_size, fl_size count);

fl_void fl_mem_copy(const fl_unknown *src, fl_unknown *dest, fl_size size);

fl_void fl_mem_zero(fl_unknown *memory, fl_size count);