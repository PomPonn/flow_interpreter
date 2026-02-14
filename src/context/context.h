#pragma once

#include "shared/common.h"

struct _fl_alias
{
    const fl_char *name;
    fl_unknown *target;
};
typedef struct _fl_alias fl_alias;

// struct _fl_context
// {
// };
// typedef struct _fl_context fl_context;