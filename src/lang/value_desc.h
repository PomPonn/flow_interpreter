/**
 * ---- value_desc.h ----
 *
 * Declares data type description logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "lang/data_type.h"

// ---- TYPE DEFINITIONS ----

struct _fl_value_desc
{
    fl_data_type type;
    fl_bool refable;
};
typedef struct _fl_value_desc fl_value_desc;

FL_HEADER_FN fl_value_desc fl_value_desc_create(
    fl_data_type type,
    fl_bool refable)
{
    return (fl_value_desc){.type = type, .refable = refable};
}

FL_HEADER_FN fl_bool fl_value_desc_equal(
    const fl_value_desc vd1,
    const fl_value_desc vd2)
{
    return fl_data_type_has(vd1.type, vd2.type) &&
           vd1.refable == vd2.refable;
}