/**
 * ---------- fn_traits.h --------------------
 *
 * Declares function traits logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_fn_traits
{
    // Pure functions give optimalization opportunities during compilation.
    fl_bool is_pure;
};
typedef struct _fl_fn_traits fl_fn_traits;

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_fn_traits fl_fn_traits_create(fl_bool is_pure);

FL_HEADER_FN fl_fn_traits fl_fn_traits_create_empty();

// ---------- DEFINITIONS -----------

FL_HEADER_FN fl_fn_traits fl_fn_traits_create(fl_bool is_pure)
{
    return (fl_fn_traits){
        .is_pure = is_pure,
    };
}

FL_HEADER_FN fl_fn_traits fl_fn_traits_create_empty()
{
    return (fl_fn_traits){
        .is_pure = FL_FALSE,
    };
}