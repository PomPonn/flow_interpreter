/**
 * ---------- cmp_context.h ---------------
 *
 * Declares compilation context logic.
 *
 * ----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "context/local_context.h"
#include "vecs/byte_vec.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_cmp_context
{
    // Context base scope.
    fl_base_context *base;

    // Current context scope.
    fl_scope *scope;

    // Byte code portion associated with this context.
    fl_byte_vec byte_code;
};

/**
 * @brief Compilation context.
 */
typedef struct _fl_cmp_context fl_cmp_context;

// ---------- DECLARATIONS --------------

/**
 * @brief Initialize compilation context.
 * @param cnt Uninitialized context to init.
 * @param base Base context to connect to this context.
 */
FL_SEALED_FN fl_void fl_cmp_context_init(fl_cmp_context *cnt, fl_base_context *base);

/**
 * @brief Initialize compilation context from given local context.
 * @param cnt Uninitialized context to init.
 * @param ref Local context to use.
 */
FL_SEALED_FN fl_void fl_cmp_context_from(fl_cmp_context *cnt, const fl_local_context *ref);

/**
 * @brief Free compilation context.
 * @param cnt Context to free.
 */
FL_SEALED_FN fl_void fl_cmp_context_free(fl_cmp_context *cnt);

// ---------- DEFINITIONS --------------

FL_SEALED_FN fl_void fl_cmp_context_free(fl_cmp_context *cnt)
{
    fl_byte_vec_free(&cnt->byte_code);
    cnt->base = FL_NULL;
    cnt->scope = FL_NULL;
}

FL_SEALED_FN fl_void fl_cmp_context_from(fl_cmp_context *cnt, const fl_local_context *ref)
{
    fl_cmp_context_init(cnt, ref->base);
    cnt->scope = ref->scope;
}

FL_SEALED_FN fl_void fl_cmp_context_init(fl_cmp_context *cnt, fl_base_context *base)
{
    cnt->base = base;
    cnt->scope = base->main_scope;
    fl_byte_vec_init(&cnt->byte_code);
}