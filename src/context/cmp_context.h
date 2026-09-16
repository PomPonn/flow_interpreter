/**
 * ---------- cmp_context.h ---------------
 *
 * Declares compilation context logic.
 *
 * ----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "vecs/byte_vec.h"
#include "context/local_context.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_cmp_context
{
    fl_base_context *base;
    fl_scope *scope;
    fl_byte_vec byte_code;
};
/**
 * @brief Describes compilationn cmp_context.
 */
typedef struct _fl_cmp_context fl_cmp_context;

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_void fl_cmp_context_init(fl_cmp_context *cnt, fl_base_context *base);
FL_HEADER_FN fl_void fl_cmp_context_from(fl_cmp_context *cnt, const fl_local_context *ref);
FL_HEADER_FN fl_void fl_cmp_context_free(fl_cmp_context *cnt);

// ---------- DEFINITIONS --------------

FL_HEADER_FN fl_void fl_cmp_context_free(fl_cmp_context *cnt)
{
    fl_byte_vec_free(&cnt->byte_code);
}

FL_HEADER_FN fl_void fl_cmp_context_from(fl_cmp_context *cnt, const fl_local_context *ref)
{
    fl_cmp_context_init(cnt, ref->base);
    cnt->scope = ref->scope;
}

FL_HEADER_FN fl_void fl_cmp_context_init(fl_cmp_context *cnt, fl_base_context *base)
{
    cnt->base = base;
    cnt->scope = base->main_scope;
    fl_byte_vec_init(&cnt->byte_code);
}