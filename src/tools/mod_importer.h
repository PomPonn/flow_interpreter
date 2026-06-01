#pragma once

#include "shared/common.h"

FL_FORWARD_STRUCT(fl_base_context);
FL_FORWARD_STRUCT(fl_value);

fl_value fl_mimp_import_id_value(const fl_char *id_str, fl_base_context *cnt);