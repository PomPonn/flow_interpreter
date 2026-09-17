/**
 * ---------- token_location.h ---------------
 *
 * Declares token location logic.
 *
 * -------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- MACROS -------------------

#define FL_TOKEN_LOCATION_EMPTY fl_token_location_create("empty", 0, 0)

// ---------- TYPE DEFINITIONS ----------

struct _fl_token_location
{
    const fl_char *description;
    fl_size line;
    fl_size column;
};
typedef struct _fl_token_location fl_token_location;

// ---------- DECLARATIONS --------------

FL_SEALED_FN fl_token_location fl_token_location_create(
    const fl_char *description,
    fl_size line,
    fl_size column);

// ---------- DEFINITIONS -----------

FL_SEALED_FN fl_token_location fl_token_location_create(
    const fl_char *description,
    fl_size line,
    fl_size column)
{
    return (fl_token_location){
        .description = description,
        .line = line,
        .column = column,
    };
}
