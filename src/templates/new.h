/**
 * ---------- new.h -----------------------
 *
 * Implements new memory allocation
 * template logic.
 *
 *
 * ----------------------------------------
 */

// ---------- DEPENDENCIES --------------

#include "macros/basic.h"
#include "shared/memory.h"

// ------------- MACROS -----------------

#define FL_NEW_MEM_FN_OF(type)                              \
    FL_INLINE_FN type *_FL_MERGE2(fl_new_, type)(type data) \
    {                                                       \
        type *v = fl_mem_alloc_t(type);                     \
        *v = data;                                          \
        return v;                                           \
    }