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

struct _fl_fn_intf
{
    fl_data_type return_type;
    fl_fn_traits traits;
    fl_data_desc_vec param_descs;
    fl_cstr_vec param_labels;
};
typedef struct _fl_fn_intf fl_fn_intf;

// ---------- DECLARATIONS ----------

FL_SEALED_FN fl_void fl_fn_intf_free(fl_fn_intf *intf);
FL_SEALED_FN fl_void fl_fn_intf_set_labels(fl_fn_intf *intf, fl_cstr_vec labels);
FL_SEALED_FN fl_size fl_fn_intf_mem_size(const fl_fn_intf intf);

FL_SEALED_FN fl_void fl_fn_intf_init(
    fl_fn_intf *intf,
    fl_data_type return_type,
    fl_data_desc_vec param_descs);

fl_void fl_fn_intf_to_bytes(const fl_fn_intf intf, fl_byte_vec *byte_vec);
fl_bool fl_fn_intf_overlaps(fl_fn_intf left, fl_fn_intf right);

// ---------- DEFINITIONS ----------

FL_SEALED_FN fl_void fl_fn_intf_init(
    fl_fn_intf *intf,
    fl_data_type return_type,
    fl_data_desc_vec param_descs)
{
    intf->return_type = return_type;
    intf->traits = FL_FN_TRAITS_NONE;
    intf->param_descs = param_descs;
    fl_cstr_vec_init_empty(&intf->param_labels);
}

FL_SEALED_FN fl_void fl_fn_intf_set_labels(fl_fn_intf *intf, fl_cstr_vec labels)
{
    assert(intf->param_descs.size == labels.size);
    intf->param_labels = labels;
}

FL_SEALED_FN fl_void fl_fn_intf_free(fl_fn_intf *intf)
{
    fl_data_desc_vec_free(&intf->param_descs);
    fl_cstr_vec_free(&intf->param_labels);
    intf->return_type = FL_DATA_TYPE_NULL;
    intf->traits = FL_FN_TRAITS_NONE;
}

FL_SEALED_FN fl_size fl_fn_intf_mem_size(const fl_fn_intf intf)
{
    return sizeof(fl_data_type) + sizeof(fl_fn_traits) + intf.param_descs.size * sizeof(fl_data_desc);
}
