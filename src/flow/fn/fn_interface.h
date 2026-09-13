/**
 * ---------- fn_interface.h ---------------
 *
 * Declares function intf logic.
 *
 * -----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES -------------

#include "vecs/data_desc_vec.h"
#include "flow/fn/fn_traits.h"
#include "flow/data_type.h"
#include "vecs/cstr_vec.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_fn_interface
{
    fl_data_type return_type;
    fl_fn_traits traits;
    fl_data_desc_vec param_descs;
    fl_cstr_vec param_labels;
};
typedef struct _fl_fn_interface fl_fn_interface;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_void fl_fn_interface_free(fl_fn_interface *intf);
FL_HEADER_FN fl_bool fl_fn_interface_overlaps(fl_fn_interface left, fl_fn_interface right);
FL_HEADER_FN fl_size fl_fn_interface_mem_size(const fl_fn_interface intf);

FL_HEADER_FN fl_void fl_fn_interface_init(
    fl_fn_interface *intf,
    fl_data_type return_type,
    fl_data_desc_vec param_descs);

FL_HEADER_FN fl_void fl_fn_interface_to_bytes(
    const fl_fn_interface intf,
    fl_byte_vec *byte_vec);

// ---------- DEFINITIONS ----------

FL_HEADER_FN fl_void fl_fn_interface_init(
    fl_fn_interface *intf,
    fl_data_type return_type,
    fl_data_desc_vec param_descs)
{
    intf->return_type = return_type;
    intf->traits = FL_FN_TRAITS_NONE;
    intf->param_descs = param_descs;
    fl_cstr_vec_init_s(&intf->param_labels, param_descs.size);
}

FL_HEADER_FN fl_void fl_fn_interface_free(fl_fn_interface *intf)
{
    fl_data_desc_vec_free(&intf->param_descs);
    fl_cstr_vec_free(&intf->param_labels);
    intf->return_type = FL_DATA_TYPE_NULL;
    intf->traits = FL_FN_TRAITS_NONE;
}

FL_HEADER_FN fl_bool fl_fn_interface_overlaps(fl_fn_interface left, fl_fn_interface right)
{
    if (left.param_descs.size == 0 && right.param_descs.size == 0)
        return FL_TRUE;

    if (left.param_descs.size != right.param_descs.size)
        return FL_FALSE;

    for (fl_size i = 0; i < left.param_descs.size; i++)
        if (!fl_data_desc_match_other(left.param_descs.data[i], right.param_descs.data[i]))
            return FL_FALSE;

    return FL_TRUE;
}

FL_HEADER_FN fl_size fl_fn_interface_mem_size(const fl_fn_interface intf)
{
    return sizeof(fl_data_type) + sizeof(fl_fn_traits) + intf.param_descs.size * sizeof(fl_data_desc);
}

FL_HEADER_FN fl_void fl_fn_interface_to_bytes(
    const fl_fn_interface intf,
    fl_byte_vec *byte_vec)
{
    fl_byte_vec_push_mem(byte_vec, &intf, sizeof(fl_data_type) + sizeof(fl_fn_traits));

    for (fl_size i = 0; i < intf.param_descs.size; i++)
        fl_byte_vec_push_mem(byte_vec, &intf.param_descs.data[i], sizeof(fl_data_desc));
}