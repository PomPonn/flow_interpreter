/**
 * ---------- param.h ---------------
 *
 * Declares parameter logic.
 *
 * ----------------------------------
 */

#pragma once

// ---------- DEPENDENCIES -------------

#include "flow/data_type.h"
#include "flow/value.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_param_traits
{
    FL_PARAM_TRAITS_NONE = 0b00000000,
    FL_PARAM_TRAITS_REF = 0b00000001,
    FL_PARAM_TRAITS_CONST = 0b00000010,
    // ???
    // fl_param_TRAIT_EXPAND = 0b0000100,
};
typedef fl_byte fl_param_traits;

struct _fl_param
{
    fl_data_type type;
    fl_param_traits traits;
};
typedef struct _fl_param fl_param;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_param fl_param_create(fl_data_type type);
FL_HEADER_FN fl_param fl_param_create_traited(fl_data_type type, fl_param_traits traits);

FL_HEADER_FN fl_bool fl_param_equal(fl_param vd1, fl_param vd2);
FL_HEADER_FN fl_bool fl_param_match_arg(fl_param p, fl_value arg);
FL_HEADER_FN fl_bool fl_param_match_param(fl_param p, fl_param p2);

// ---------- DEFINITIONS -----------

FL_HEADER_FN fl_param fl_param_create(fl_data_type type)
{
    return (fl_param){
        .type = type,
        .traits = FL_PARAM_TRAITS_NONE,
    };
}

FL_HEADER_FN fl_param fl_param_create_traited(fl_data_type type, fl_param_traits traits)
{
    return (fl_param){
        .type = type,
        .traits = traits,
    };
}

FL_HEADER_FN fl_bool fl_param_equal(fl_param p1, fl_param p2)
{
    return fl_bitfield_is_set(p1.type, p2.type) &&
           p1.traits == p2.traits;
}

FL_HEADER_FN fl_bool fl_param_match_arg(fl_param p, fl_value arg)
{
    // Is it enough?

    if (fl_bitfield_is_set(p.traits, FL_PARAM_TRAITS_REF) && arg.data_type != FL_DATA_TYPE_REF)
        return FL_FALSE;

    if (!fl_bitfield_is_set(p.traits, FL_PARAM_TRAITS_REF) && arg.data_type == FL_DATA_TYPE_REF)
        return FL_FALSE;

    return fl_bitfield_is_any(p.type, arg.data_type);
}

FL_HEADER_FN fl_bool fl_param_match_param(fl_param p, fl_param p2)
{
    if (!fl_bitfield_is_any(p.type, p2.type))
        return FL_FALSE;

    if (fl_bitfield_is_set(p.traits, FL_PARAM_TRAITS_CONST) &&
        !fl_bitfield_is_set(p2.traits, FL_PARAM_TRAITS_CONST))
        return FL_FALSE;

    if (fl_bitfield_is_set(p.traits, FL_PARAM_TRAITS_REF) &&
        !fl_bitfield_is_set(p2.traits, FL_PARAM_TRAITS_REF))
        return FL_FALSE;

    return FL_TRUE;
}