/**
 * ---------- fn_variant.h -----------------------
 *
 * Declares function variant (overload) logic.
 *
 * -----------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "vecs/data_desc_vec.h"
#include "vecs/value_vec.h"
#include "vecs/byte_vec.h"
#include "flow/fn/fn_interface.h"
#include "flow/byte_segm.h"

// ---------- TYPE DEFINITIONS ----------

typedef fl_value (*fl_c_fn_ptr)(fl_value_vec args);

struct _fl_fn_variant
{
    // Whether variant is defined in C or Flow.
    fl_bool c_defined;

    // Interface of this function variant.
    fl_fn_intf intf;

    union
    {
        // Pointer to C function definition.
        fl_c_fn_ptr fn_ptr;

        // Pointer to Flow function body.
        fl_byte_segm *body_ptr;
    };

    static_assert(sizeof(fl_c_fn_ptr) == sizeof(fl_byte_vec *), "fl_fn_variant union mismatch");
};

/**
 * @brief Callable function variant (overload).
 */
typedef struct _fl_fn_variant fl_fn_variant;

// ---------- DECLARATIONS --------------

/**
 * @brief Free function variant.
 * @param var Variant to free.
 */
FL_SEALED_FN fl_void fl_fn_variant_free(fl_fn_variant *var);

/**
 * @brief Check if two function variants overlap.
 * @param left First one.
 * @param right Second one.
 * @return Boolean value whether it's overlapping.
 */
FL_SEALED_FN fl_bool fl_fn_variant_overlaps(const fl_fn_variant *left, const fl_fn_variant *right);

/**
 * @brief Initialize function variant.
 * @param var Uninitialized variant to init.
 * @param c_defined Whether the variant is c defined.
 * @param intf Variant interface. Becomes owned by this variant.
 * @param cll_ptr Pointer to callable segment of the variant. If C defined, it's a pointer to the function.
 *                If Flow defined, it's a pointer to the function body code segment.
 */
FL_SEALED_FN fl_void fl_fn_variant_init(
    fl_fn_variant *var,
    fl_bool c_defined,
    fl_fn_intf intf,
    fl_unknown *cll_ptr);

// ---------- DEFINITIONS ---------------

FL_SEALED_FN fl_void fl_fn_variant_init(
    fl_fn_variant *var,
    fl_bool c_defined,
    fl_fn_intf intf,
    fl_unknown *cll_ptr)
{

    var->c_defined = c_defined;
    var->intf = intf;

    if (c_defined)
        var->fn_ptr = cll_ptr;
    else
        var->body_ptr = cll_ptr;
}

FL_SEALED_FN fl_void fl_fn_variant_free(fl_fn_variant *var)
{
    fl_fn_intf_free(&var->intf);

    if (var->c_defined)
        var->fn_ptr = FL_NULL;
    else
        var->body_ptr = FL_NULL;

    var->c_defined = FL_FALSE;
}

FL_SEALED_FN fl_bool fl_fn_variant_overlaps(const fl_fn_variant *left, const fl_fn_variant *right)
{
    return fl_fn_intf_overlaps(&left->intf, &right->intf);
}