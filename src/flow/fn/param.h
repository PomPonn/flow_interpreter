/**
 * ---------- param.h ----------
 *
 * Declares parameter description logic.
 *
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "flow/data_type.h"
#include "flow/value.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_param
{
    fl_data_type type;
    fl_bool refering;
    fl_bool expands;
};
typedef struct _fl_param fl_param;

FL_HEADER_FN fl_param fl_param_create(fl_data_type type);
FL_HEADER_FN fl_param fl_param_create_ref(fl_data_type type);
FL_HEADER_FN fl_param fl_param_create_exp(fl_data_type type);
FL_HEADER_FN fl_bool fl_param_equal(fl_param vd1, fl_param vd2);
FL_HEADER_FN fl_bool fl_param_suits_arg(fl_param p, fl_value arg);

FL_HEADER_FN fl_param fl_param_create(fl_data_type type)
{
    return (fl_param){.type = type, .refering = FL_FALSE, .expands = FL_FALSE};
}

FL_HEADER_FN fl_param fl_param_create_ref(fl_data_type type)
{
    return (fl_param){.type = type, .refering = FL_TRUE, .expands = FL_FALSE};
}

FL_HEADER_FN fl_param fl_param_create_exp(fl_data_type type)
{
    return (fl_param){.type = type, .refering = FL_FALSE, .expands = FL_TRUE};
}

FL_HEADER_FN fl_bool fl_param_equal(fl_param p1, fl_param p2)
{
    return fl_data_type_has(p1.type, p2.type) &&
           p1.refering == p2.refering;
}

FL_HEADER_FN fl_bool fl_param_suits_arg(fl_param p, fl_value arg)
{
    // Is it enough?
    return fl_data_type_has(p.type, arg.data_type);
}