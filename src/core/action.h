#pragma once

#include "shared/common.h"

enum _fl_action_type
{
    FL_ACTION_TYPE_ARITHMETIC,
    FL_ACTION_TYPE_FUNCTION,
};
typedef fl_byte fl_action_type;

struct _fl_action
{
    fl_action_type type;
};
typedef struct _fl_action fl_action;