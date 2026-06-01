#pragma once

#include "shared/common.h"

FL_FORWARD_STRUCT(fl_value);
FL_FORWARD_STRUCT(fl_scope);
FL_FORWARD_STRUCT(fl_id_chain);

fl_value fl_std_io_register(fl_scope *scope, const fl_id_chain *id_chain);