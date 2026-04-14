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
    // to change
    union
    {
        fl_c_fn_ptr pointer;
        fl_byte_vec body;
    };
};
/**
 * @brief Function variant;
 * contains information about function variant (overload).
 */
typedef struct _fl_fn_variant fl_fn_variant;

FL_HEADER_FN fl_void fl_fn_variant_init(
    fl_fn_variant *variant,
    fl_bool c_defined,
    fl_fn_interface interface);
FL_HEADER_FN fl_void fl_fn_variant_free(fl_fn_variant *variant);
FL_HEADER_FN fl_value fl_fn_variant_call(fl_fn_variant *variant, fl_value_vec args);
FL_HEADER_FN fl_bool fl_fn_variant_overlaps(fl_fn_variant left, fl_fn_variant right);

FL_HEADER_FN fl_void
fl_fn_variant_init(
    fl_fn_variant *variant,
    fl_bool c_defined,
    fl_fn_interface interface)
{
    variant->c_defined = c_defined;
    variant->interface = interface;
    fl_mem_zero(&variant->body, sizeof(fl_byte_vec));
}

FL_HEADER_FN fl_void fl_fn_variant_free(fl_fn_variant *variant)
{
    fl_fn_interface_free(&variant->interface);

    if (variant->c_defined)
        fl_byte_vec_free(&variant->body);
}

FL_HEADER_FN fl_value fl_fn_variant_call(fl_fn_variant *variant, fl_value_vec args)
{
    return variant->c_defined ? variant->pointer(args) : FL_VALUE_EMPTY; // variant->body ??
}

FL_HEADER_FN fl_bool fl_fn_variant_overlaps(fl_fn_variant left, fl_fn_variant right)
{
    return fl_fn_interface_overlaps(left.interface, right.interface);
}