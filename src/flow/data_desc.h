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
#include "flow/value.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_data_traits
{
    FL_DATA_TRAITS_NONE = 0b00000000,
    FL_DATA_TRAITS_REF = 0b00000001,
    FL_DATA_TRAITS_CONST = 0b00000010,
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

FL_HEADER_FN fl_data_desc fl_data_desc_create(fl_data_type type);
FL_HEADER_FN fl_data_desc fl_data_desc_create_traited(fl_data_type type, fl_data_traits traits);

FL_HEADER_FN fl_bool fl_data_desc_equal(fl_data_desc vd1, fl_data_desc vd2);
FL_HEADER_FN fl_bool fl_data_desc_match_value(fl_data_desc p, fl_value arg);
FL_HEADER_FN fl_bool fl_data_desc_match_other(fl_data_desc p, fl_data_desc p2);

// ---------- DEFINITIONS -----------

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

FL_HEADER_FN fl_bool fl_data_desc_equal(fl_data_desc p1, fl_data_desc p2)
{
    return fl_bitfield_is_set(p1.type, p2.type) &&
           p1.traits == p2.traits;
}

FL_HEADER_FN fl_bool fl_data_desc_match_value(fl_data_desc p, fl_value arg)
{
    // Is it enough?

    if (fl_bitfield_is_set(p.traits, FL_DATA_TRAITS_REF) && arg.data_type != FL_DATA_TYPE_REF)
        return FL_FALSE;

    if (!fl_bitfield_is_set(p.traits, FL_DATA_TRAITS_REF) && arg.data_type == FL_DATA_TYPE_REF)
        return FL_FALSE;

    return fl_bitfield_is_any(p.type, arg.data_type);
}

FL_HEADER_FN fl_bool fl_data_desc_match_other(fl_data_desc p, fl_data_desc p2)
{
    if (!fl_bitfield_is_any(p.type, p2.type))
        return FL_FALSE;

    if (fl_bitfield_is_set(p.traits, FL_DATA_TRAITS_CONST) &&
        !fl_bitfield_is_set(p2.traits, FL_DATA_TRAITS_CONST))
        return FL_FALSE;

    if (fl_bitfield_is_set(p.traits, FL_DATA_TRAITS_REF) &&
        !fl_bitfield_is_set(p2.traits, FL_DATA_TRAITS_REF))
        return FL_FALSE;

    return FL_TRUE;
}