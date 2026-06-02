/**
 * ---------- memory.h --------------------
 *
 * Declares memory managament logic.
 *
 * ----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/types.h"

// ------------- MACROS -----------------

#define fl_mem_alloc_t(type) fl_mem_alloc(sizeof(type))
#define fl_mem_alloc_arr_t(type, count) fl_mem_alloc_arr(sizeof(type), count)
#define fl_mem_realloc_arr_t(mem, elem_type, new_size) \
    fl_mem_realloc_arr(mem, sizeof(elem_type), new_size)

// ---------- DECLARATIONS --------------

fl_void fl_mem_free(fl_unknown *memory);
fl_unknown *fl_mem_alloc(fl_size size);
fl_unknown *fl_mem_realloc_arr(fl_unknown *mem, fl_size elem_size, fl_size new_size);
fl_unknown *fl_mem_alloc_arr(fl_size elem_size, fl_size count);
fl_void fl_mem_copy(const fl_unknown *src, fl_unknown *dest, fl_size size);
fl_void fl_mem_zero(fl_unknown *memory, fl_size count);
