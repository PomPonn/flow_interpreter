/**
 * ---------- param.h ----------
 *
 * Declares parameter description logic.
 *
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "flow/data_type.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_param
{
    fl_data_type type;
    fl_bool refering;
    fl_bool expands;
};
typedef struct _fl_param fl_param;

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

FL_HEADER_FN fl_bool fl_param_equal(const fl_param vd1, const fl_param vd2)
{
    return fl_data_type_has(vd1.type, vd2.type) &&
           vd1.refering == vd2.refering;
}