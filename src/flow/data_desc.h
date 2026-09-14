/**
 * ---------- data_desc.h ---------------
 *
 * Declares data descriptor logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES -------------

#include "flow/data_type.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_data_traits
{
    FL_DATA_TRAITS_NONE = 0b00000000,
    FL_DATA_TRAITS_CONST = 0b00000001,
    FL_DATA_TRAITS_IMMUTABLE = 0b00000010,
    // ???
    // FL_DATA_TRAITS_EXPAND = 0b0000100,
};
typedef fl_byte fl_data_traits;

struct _fl_data_desc
{
    fl_data_type type;
    fl_data_traits traits;
};
typedef struct _fl_data_desc fl_data_desc;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_data_desc fl_data_desc_empty();
FL_HEADER_FN fl_data_desc fl_data_desc_create(fl_data_type type);
FL_HEADER_FN fl_data_desc fl_data_desc_create_traited(fl_data_type type, fl_data_traits traits);

FL_HEADER_FN fl_bool fl_data_desc_equal(fl_data_desc d1, fl_data_desc d2);
FL_HEADER_FN fl_bool fl_data_desc_match_other(fl_data_desc d1, fl_data_desc d2);

// ---------- DEFINITIONS -----------

FL_HEADER_FN fl_data_desc fl_data_desc_empty()
{
    return (fl_data_desc){
        .type = FL_DATA_TYPE_NULL,
        .traits = FL_DATA_TRAITS_NONE,
    };
}

FL_HEADER_FN fl_data_desc fl_data_desc_create(fl_data_type type)
{
    return (fl_data_desc){
        .type = type,
        .traits = FL_DATA_TRAITS_NONE,
    };
}

FL_HEADER_FN fl_data_desc fl_data_desc_create_traited(fl_data_type type, fl_data_traits traits)
{
    return (fl_data_desc){
        .type = type,
        .traits = traits,
    };
}

FL_HEADER_FN fl_bool fl_data_desc_equal(fl_data_desc d1, fl_data_desc d2)
{
    return fl_bitfield_is_set(d1.type, d2.type) &&
           d1.traits == d2.traits;
}

FL_HEADER_FN fl_bool fl_data_desc_match_other(fl_data_desc d1, fl_data_desc d2)
{
    // TODO: change this

    if (!fl_bitfield_is_any(d1.type, d2.type))
        return FL_FALSE;

    if (fl_bitfield_is_set(d1.traits, FL_DATA_TRAITS_CONST) &&
        !fl_bitfield_is_set(d2.traits, FL_DATA_TRAITS_CONST))
        return FL_FALSE;

    // if (fl_bitfield_is_set(d1.type, FL_DATA_TYPE_REF) &&
    //     !fl_bitfield_is_set(d2.type, FL_DATA_TYPE_REF))
    //     return FL_FALSE;

    return FL_TRUE;
}