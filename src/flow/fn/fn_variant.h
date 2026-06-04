/**
 * ---------- fn_variant.h -----------------------
 *
 * Declares function variant (overload) logic.
 *
 * -----------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "vecs/param_vec.h"
#include "vecs/value_vec.h"
#include "vecs/byte_vec.h"
#include "flow/fn/fn_interface.h"

// ---------- TYPE DEFINITIONS ----------

typedef fl_value (*fl_c_fn_ptr)(fl_value_vec args);

struct _fl_fn_variant
{
    fl_bool c_defined;
    fl_fn_interface interface;
    // TODO: revise this
    union
    {
        fl_c_fn_ptr pointer;
        fl_byte_vec body;
    };
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
    fl_fn_interface interface);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_void fl_fn_variant_init(
    fl_fn_variant *var,
    fl_bool c_defined,
    fl_fn_interface interface)
{
    var->c_defined = c_defined;
    var->interface = interface;
    fl_mem_zero(&var->body, sizeof(fl_byte_vec));
}

FL_HEADER_FN fl_void fl_fn_variant_free(fl_fn_variant *var)
{
    fl_fn_interface_free(&var->interface);

    if (var->c_defined)
        fl_byte_vec_free(&var->body);
    else
        var->pointer = FL_NULL;

    var->c_defined = FL_FALSE;
}

FL_HEADER_FN fl_value fl_fn_variant_call(fl_fn_variant *var, fl_value_vec args)
{
    // var->body ???
    return var->c_defined ? var->pointer(args) : FL_VALUE_EMPTY;
}

FL_HEADER_FN fl_bool fl_fn_variant_overlaps(fl_fn_variant left, fl_fn_variant right)
{
    return fl_fn_interface_overlaps(left.interface, right.interface);
}