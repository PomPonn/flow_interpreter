/**
 * ---------- fn_variant.h -----------------------
 *
 * Declares function variant (overload) logic.
 *
 * -----------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "vecs/data_desc_vec.h"
#include "vecs/value_vec.h"
#include "vecs/byte_vec.h"
#include "flow/fn/fn_interface.h"
#include "flow/byte_segm.h"

// ---------- TYPE DEFINITIONS ----------

typedef fl_value (*fl_c_fn_ptr)(fl_value_vec args);

struct _fl_fn_variant
{
    fl_bool c_defined;
    fl_fn_interface interface;
    // TODO: revise this
    union
    {
        fl_c_fn_ptr fn_ptr;
        fl_byte_segm *body_ptr;
    };

    static_assert(sizeof(fl_c_fn_ptr) == sizeof(fl_byte_vec *), "fl_fn_variant union mismatch");
};

/**
 * @brief Function callable;
 * Contains information about function callable (overload).
 */
typedef struct _fl_fn_variant fl_fn_variant;

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_void fl_fn_variant_free(fl_fn_variant *var);
FL_HEADER_FN fl_value fl_fn_variant_call(fl_fn_variant *var, fl_value_vec args);
FL_HEADER_FN fl_bool fl_fn_variant_overlaps(fl_fn_variant left, fl_fn_variant right);
FL_HEADER_FN fl_void fl_fn_variant_init(
    fl_fn_variant *var,
    fl_bool c_defined,
    fl_fn_interface interface,
    fl_unknown *cll_ptr);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_void fl_fn_variant_init(
    fl_fn_variant *var,
    fl_bool c_defined,
    fl_fn_interface interface,
    fl_unknown *cll_ptr)
{

    var->c_defined = c_defined;
    var->interface = interface;

    if (c_defined)
        var->fn_ptr = cll_ptr;
    else
        var->body_ptr = cll_ptr;
}

FL_HEADER_FN fl_void fl_fn_variant_free(fl_fn_variant *var)
{
    fl_fn_interface_free(&var->interface);

    if (var->c_defined)
        var->fn_ptr = FL_NULL;
    else
        var->body_ptr = FL_NULL;

    var->c_defined = FL_FALSE;
}

FL_HEADER_FN fl_value fl_fn_variant_call(fl_fn_variant *var, fl_value_vec args)
{
    return var->c_defined ? var->fn_ptr(args) : fl_value_empty();
}

FL_HEADER_FN fl_bool fl_fn_variant_overlaps(fl_fn_variant left, fl_fn_variant right)
{
    return fl_fn_interface_overlaps(left.interface, right.interface);
}