#pragma once

#include "shared/common.h"

FL_FORWARD_STRUCT(fl_value);
FL_FORWARD_STRUCT(fl_id_chain);

FL_FLOW_MODULE fl_value fl_module_resolve_id_value(const fl_id_chain *id_chain);