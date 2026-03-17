#pragma once

#include "shared/common.h"

struct _fl_file_location_info
{
    const fl_char *description;
    fl_size line;
    fl_size column;
};
typedef struct _fl_file_location_info fl_file_location_info;