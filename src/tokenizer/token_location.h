#pragma once

#include "shared/common.h"

#define FL_TOKEN_LOCATION_EMPTY fl_token_location_create("empty", 0, 0)

struct _fl_token_location
{
    const fl_char *description;
    fl_size line;
    fl_size column;
};
typedef struct _fl_token_location fl_token_location;

FL_HEADER_FN fl_token_location fl_token_location_create(
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