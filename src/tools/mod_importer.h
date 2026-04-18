#pragma once

#include "shared/common.h"

FL_FORWARD_STRUCT(fl_context);
FL_FORWARD_STRUCT(fl_value);

fl_value fl_mimp_import_id_value(fl_context *cnt, const fl_char *id_str);