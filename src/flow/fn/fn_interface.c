/**
 * ---------- fn_interface.c ----------------
 *
 * Implements function interface logic.
 *
 * ------------------------------------------
 */

#include "fn_interface.h"

// ---------- DEPENDENCIES ------------

#include "vecs/byte_vec.h"

// ---------- DEFINITIONS -------------

fl_bool fl_fn_intf_overlaps(const fl_fn_intf *left, const fl_fn_intf *right)
{
    if (left->param_descs.size == 0 && right->param_descs.size == 0)
        return FL_TRUE;

    if (left->param_descs.size != right->param_descs.size)
        return FL_FALSE;

    for (fl_size i = 0; i < left->param_descs.size; i++)
        if (!fl_data_desc_match_other(left->param_descs.data[i], right->param_descs.data[i]))
            return FL_FALSE;

    return FL_TRUE;
}

fl_void fl_fn_intf_to_bytes(const fl_fn_intf *intf, fl_byte_vec *byte_vec)
{
    // !!! TEMP

    // other intf fields...

    const fl_size n = intf->param_labels.size;
    fl_byte_vec_push_mem(byte_vec, &n, sizeof(fl_size));

    for (fl_size i = 0; i < n; i++)
    {
        fl_char *label = intf->param_labels.data[i];
        fl_byte_vec_push_str(byte_vec, label);

        // param traits...
    }
}