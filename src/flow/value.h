/**
 * ---------- value.h ---------------
 *
 * Declares id value logic.
 *
 * ----------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "flow/data_type.h"

// ---------- MACROS ----------------

#define FL_VALUE_EMPTY fl_value_create(FL_DATA_TYPE_NULL, FL_NULL)

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_byte_reader);
FL_FORWARD_STRUCT(fl_byte_vec);

struct _fl_value
{
    fl_data_type data_type;
    fl_unknown *data;
};
typedef struct _fl_value fl_value;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_value fl_value_create(fl_data_type type, fl_unknown *data);
FL_HEADER_FN fl_bool fl_value_equal(fl_value v1, fl_value v2);
FL_HEADER_FN fl_bool fl_value_is_size_complex(fl_value v);

fl_size fl_value_mem_size(fl_value value);
fl_void fl_value_free(fl_value *value);
fl_value fl_value_from_literal(fl_data_type lit_type, fl_unknown *lit_data);
fl_bool fl_value_indentical(fl_value v1, fl_value v2);
fl_value fl_value_copy(fl_value value);
fl_void fl_value_to_bytes(fl_value v, fl_byte_vec *bytes);
fl_value fl_value_from_bytes(fl_byte_reader *reader);

// ---------- DEFINITIONS ----------

FL_HEADER_FN fl_value fl_value_create(fl_data_type type, fl_unknown *data)
{
    return (fl_value){
        .data = data,
        .data_type = type,
    };
}

FL_HEADER_FN fl_bool fl_value_equal(fl_value v1, fl_value v2)
{
    return v1.data_type == v2.data_type && v1.data == v2.data;
}

FL_HEADER_FN fl_bool fl_value_is_empty(fl_value v)
{
    return v.data == FL_NULL;
}

FL_HEADER_FN fl_bool fl_value_is_size_complex(fl_value v)
{
    switch (v.data_type)
    {
    case FL_DATA_TYPE_ARRAY:
    case FL_DATA_TYPE_FN:
    case FL_DATA_TYPE_SCOPE:
        return FL_TRUE;
    default:
        return FL_FALSE;
    }
}