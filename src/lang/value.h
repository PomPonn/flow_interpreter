/**
 * ---- value.h ----
 *
 * Declares id value logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "lang/data_type.h"

// ---- MACROS ----

#define FL_VALUE_EMPTY fl_value_create(FL_DATA_TYPE_NULL, FL_NULL)

// ---- TYPE DEFINITIONS ----

struct _fl_value
{
    fl_data_type data_type;
    fl_unknown *data;
};
typedef struct _fl_value fl_value;

// ---- DECLARATIONS ----

FL_HEADER_FN fl_value fl_value_create(fl_data_type type, fl_unknown *data);

fl_void fl_value_free(fl_value *value);
fl_value fl_value_from_literal(fl_data_type lit_type, fl_unknown *lit_data);

// ---- HEADER-ONLY DEFINITIONS ----

FL_HEADER_FN fl_value fl_value_create(fl_data_type type, fl_unknown *data)
{
    return (fl_value){
        .data = data,
        .data_type = type,
    };
}