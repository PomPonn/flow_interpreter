#pragma once

#include "shared/common.h"

void fl_mem_free(fl_unknown *memory);

fl_unknown *fl_mem_alloc(fl_size size);

fl_unknown *fl_mem_alloc_arr(fl_size elem_size, fl_size count);
