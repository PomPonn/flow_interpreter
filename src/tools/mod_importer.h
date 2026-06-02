/**
 * ---------- mod_importer.h -------------
 *
 * Declares modules imported logic.
 *
 * ---------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_base_context);
FL_FORWARD_STRUCT(fl_value);

// ---------- DECLARATIONS --------------

fl_value fl_mimp_import_id_value(const fl_char *id_str, fl_base_context *cnt);
