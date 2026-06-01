/**
 * ---------- param.h ----------
 *
 * Declares parameter description logic.
 *
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "shared/bitfield.h"
#include "flow/data_type.h"
#include "flow/value.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_param_traits
{
    FL_PARAM_TRAITS_NONE = 0b00000000,
    FL_PARAM_TRAITS_REF = 0b00000010,
    FL_PARAM_TRAITS_CONST = 0b00000100,
    // ???
    // FL_PARAM_TRAIT_EXPAND = 0b00000010,
};
typedef fl_byte fl_param_traits;

struct _fl_param
{
    fl_data_type type;
    fl_param_traits traits;
};
typedef struct _fl_param fl_param;

FL_HEADER_FN fl_param fl_param_create(fl_data_type type);
FL_HEADER_FN fl_param fl_param_create_traited(fl_data_type type, fl_param_traits traits);

FL_HEADER_FN fl_bool fl_param_equal(fl_param vd1, fl_param vd2);
FL_HEADER_FN fl_bool fl_param_suits_arg(fl_param p, fl_value arg);

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
    return fl_bitfield8_has(p1.type, p2.type) &&
           p1.traits == p2.traits;
}

FL_HEADER_FN fl_bool fl_param_suits_arg(fl_param p, fl_value arg)
{
    // Is it enough?
    return fl_bitfield8_has(p.type, arg.data_type);
}