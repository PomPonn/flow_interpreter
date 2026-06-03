/**
 * ---------- fn_interface.h ----------
 *
 * Declares function interface logic.
 *
 * ----------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "flow/data_type.h"
#include "vecs/param_vec.h"
#include "flow/fn/fn_traits.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_fn_interface
{
    fl_data_type return_type;
    fl_fn_traits traits;
    fl_param_vec params;
};
typedef struct _fl_fn_interface fl_fn_interface;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_void fl_fn_interface_init(
    fl_fn_interface *interface,
    fl_data_type return_type,
    fl_param_vec params);
FL_HEADER_FN fl_void fl_fn_interface_free(fl_fn_interface *interface);
FL_HEADER_FN fl_bool fl_fn_interface_overlaps(fl_fn_interface left, fl_fn_interface right);
FL_HEADER_FN fl_void fl_fn_interface_add_trait(fl_fn_interface *interface, fl_fn_traits trait);
FL_HEADER_FN fl_bool fl_fn_interface_is_const(const fl_fn_interface *interface);

// ---------- HEADER-ONLY DEFINITIONS ----------

FL_HEADER_FN fl_void fl_fn_interface_init(
    fl_fn_interface *interface,
    fl_data_type return_type,
    fl_param_vec params)
{
    interface->return_type = return_type;
    interface->params = params;
    interface->traits = FL_FN_TRAITS_NONE;
}

FL_HEADER_FN fl_void fl_fn_interface_free(fl_fn_interface *interface)
{
    fl_param_vec_free(&interface->params);
    interface->return_type = FL_DATA_TYPE_NULL;
    interface->traits = FL_FN_TRAITS_NONE;
}

FL_HEADER_FN fl_void fl_fn_interface_add_trait(fl_fn_interface *interface, fl_fn_traits trait)
{
    interface->traits |= trait;
}

FL_HEADER_FN fl_bool fl_fn_interface_overlaps(fl_fn_interface left, fl_fn_interface right)
{
    if (left.params.size == 0 && right.params.size == 0)
        return FL_TRUE;

    if (left.params.size != right.params.size)
        return FL_FALSE;

    for (fl_size i = 0; i < left.params.size; i++)
        if (left.params.data[i].type != right.params.data[i].type)
            return FL_FALSE;

    return FL_TRUE;
}

FL_HEADER_FN fl_bool fl_fn_interface_is_const(const fl_fn_interface *fn_i)
{
    return fl_bitfield_has(fn_i->traits, FL_FN_TRAITS_PURE) && fl_param_vec_is_every_const(&fn_i->params);
}