/**
 * ---------- value.h ---------------
 *
 * Declares id value logic.
 *
 * ----------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "flow/data_desc.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_byte_reader);
FL_FORWARD_STRUCT(fl_byte_vec);
FL_FORWARD_STRUCT(fl_id_entry);
FL_FORWARD_STRUCT(fl_cstr_vec);

struct _fl_value
{
    fl_data_desc desc;
    fl_unknown *data;
};
typedef struct _fl_value fl_value;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_value fl_value_empty();
FL_HEADER_FN fl_value fl_value_create(fl_data_type type, fl_unknown *data);
FL_HEADER_FN fl_value fl_value_create_desc(fl_data_desc desc, fl_unknown *data);
FL_HEADER_FN fl_bool fl_value_equal(fl_value v1, fl_value v2);
FL_HEADER_FN fl_bool fl_value_is_size_complex(fl_value v);
FL_HEADER_FN fl_bool fl_value_is_desc_by(fl_value v, fl_data_desc desc);

fl_value fl_value_create_copy(fl_data_type type, const fl_unknown *data);
fl_size fl_value_mem_size(fl_value value);
fl_void fl_value_free(fl_value *value);
fl_value fl_value_from_literal(fl_data_type lit_type, fl_unknown *lit_data);
fl_bool fl_value_indentical(fl_value v1, fl_value v2);
fl_value fl_value_copy(fl_value value);
fl_void fl_value_to_bytes(fl_value v, fl_byte_vec *bytes);
fl_value fl_value_from_bytes(fl_byte_reader *reader);
// doesn't copy vector content
fl_value fl_value_from_cstr_vec(const fl_cstr_vec vec);
fl_value fl_value_create_ref(fl_id_entry *target);

// ---------- DEFINITIONS ----------

FL_HEADER_FN fl_value fl_value_empty()
{
    return fl_value_create(FL_DATA_TYPE_NULL, FL_NULL);
}

FL_HEADER_FN fl_value fl_value_create(fl_data_type type, fl_unknown *data)
{
    return (fl_value){
        .data = data,
        .desc = fl_data_desc_create(type),
    };
}

FL_HEADER_FN fl_value fl_value_create_desc(fl_data_desc desc, fl_unknown *data)
{
    return (fl_value){
        .data = data,
        .desc = desc,
    };
}

FL_HEADER_FN fl_bool fl_value_equal(fl_value v1, fl_value v2)
{
    return v1.desc.type == v2.desc.type && v1.data == v2.data;
}

FL_HEADER_FN fl_bool fl_value_is_empty(fl_value v)
{
    return v.data == FL_NULL;
}

FL_HEADER_FN fl_bool fl_value_is_size_complex(fl_value v)
{
    switch (v.desc.type)
    {
    case FL_DATA_TYPE_ARRAY:
    case FL_DATA_TYPE_FN:
    case FL_DATA_TYPE_SCOPE:
        return FL_TRUE;
    default:
        return FL_FALSE;
    }
}

FL_HEADER_FN fl_bool fl_value_is_desc_by(fl_value v, fl_data_desc desc)
{
    // Is it enough?
    // (opt this out)

    if (fl_bits_is_set(desc.traits, FL_DATA_TRAIT_REF) &&
        !fl_bits_is_set(v.desc.traits, FL_DATA_TRAIT_REF))
        return FL_FALSE;

    if (!fl_bits_is_set(desc.traits, FL_DATA_TRAIT_REF) &&
        fl_bits_is_set(v.desc.traits, FL_DATA_TRAIT_REF))
        return FL_FALSE;

    return fl_bits_is_any(desc.type, v.desc.type);
}