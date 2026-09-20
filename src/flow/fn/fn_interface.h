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
    // Return type of described function.
    fl_data_type return_type;

    // Traits of described function.
    fl_fn_traits traits;

    // Data descriptors of function paramters.
    fl_data_desc_vec param_descs;

    // Labels of function paramters.
    fl_cstr_vec param_labels;
};

/**
 * @brief Flow function interface.
 */
typedef struct _fl_fn_intf fl_fn_intf;

// ---------- DECLARATIONS ----------

/**
 * @brief Free function interface.
 * @param intf Interface to free.
 */
FL_INLINE_FN fl_void fl_fn_intf_free(fl_fn_intf *intf);

/**
 * @brief Set interface parameter labels.
 * @param intf Interface to change labels in.
 * @param labels New labels to set. They belong to the interface struct from this point.
 */
FL_INLINE_FN fl_void fl_fn_intf_set_labels(fl_fn_intf *intf, fl_cstr_vec labels);

/**
 * @brief Init function interface.
 * @param intf Uninitialized interface to init.
 * @param return_type Function return type.
 * @param param_descs Function parameters data descriptors.
 */
FL_INLINE_FN fl_void fl_fn_intf_init(
    fl_fn_intf *intf,
    fl_data_type return_type,
    fl_data_desc_vec param_descs);

/**
 * @brief Convert function interface to bytes.
 * @param intf Interface to convert.
 * @param byte_vec Byte vector to write to.
 */
fl_void fl_fn_intf_to_bytes(const fl_fn_intf *intf, fl_byte_vec *byte_vec);

/**
 * @brief Check if two function interfaces overlap.
 * @param left First interface.
 * @param right Second interface.
 * @return Boolean value whether there is overlapping.
 */
fl_bool fl_fn_intf_overlaps(const fl_fn_intf *left, const fl_fn_intf *right);

// ---------- DEFINITIONS ----------

FL_INLINE_FN fl_void fl_fn_intf_init(
    fl_fn_intf *intf,
    fl_data_type return_type,
    fl_data_desc_vec param_descs)
{
    intf->return_type = return_type;
    intf->traits = FL_FN_TRAITS_NONE;
    intf->param_descs = param_descs;

    fl_cstr_vec_init_empty(&intf->param_labels);
}

FL_INLINE_FN fl_void fl_fn_intf_set_labels(fl_fn_intf *intf, fl_cstr_vec labels)
{
    assert(intf->param_descs.size == labels.size);

    intf->param_labels = labels;
}

FL_INLINE_FN fl_void fl_fn_intf_free(fl_fn_intf *intf)
{
    fl_data_desc_vec_free(&intf->param_descs);
    fl_cstr_vec_free(&intf->param_labels);

    intf->return_type = FL_DATA_TYPE_NULL;
    intf->traits = FL_FN_TRAITS_NONE;
}