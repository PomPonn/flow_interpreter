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
    fl_bool named;
};
typedef struct _fl_value_desc fl_value_desc;

FL_HEADER_FN fl_value_desc fl_value_desc_create(
    fl_data_type type,
    fl_bool named)
{
    return (fl_value_desc){.type = type, .named = named};
}