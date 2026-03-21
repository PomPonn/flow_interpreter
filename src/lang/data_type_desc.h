/**
 * ---- data_type_desc.h ----
 *
 * Declares data type description logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "lang/data_type.h"

// ---- DEFINES ----

#define FL_DATA_TYPE_DESC_DEFAULT(data_type) \
    ((fl_data_type_desc){.type = data_type, .modifier = FL_DATA_TYPE_MOD_NONE})

#define FL_DATA_TYPE_DESC_ASSIGNABLE(data_type) \
    ((fl_data_type_desc){.type = data_type, .modifier = FL_DATA_TYPE_MOD_ASSIGNABLE})

// ---- TYPE DEFINITIONS ----

enum _fl_data_type_mod
{
    FL_DATA_TYPE_MOD_NONE = 0b00000000,
    FL_DATA_TYPE_MOD_ASSIGNABLE = 0b00000010,
};
typedef fl_byte fl_data_type_mod;

struct _fl_data_type_desc
{
    fl_data_type type;
    fl_data_type_mod modifier;
};
typedef struct _fl_data_type_desc fl_data_type_desc;

// ---- DECLARATIONS ----

FL_HEADER_FN fl_bool fl_data_type_desc_has_mod(fl_data_type_desc td, fl_data_type_mod mod)
{
    return (td.modifier & mod) == mod;
}

FL_HEADER_FN fl_bool fl_data_type_desc_match(
    fl_data_type_desc base_td,
    fl_data_type_desc other_td)
{
    return fl_data_type_has(base_td.type, other_td.type) &&
           fl_data_type_desc_has_mod(base_td, other_td.modifier);
}
