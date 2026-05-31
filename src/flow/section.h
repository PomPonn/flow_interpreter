/**
 * ---------- section.h --------------------
 *
 * Declares byte code section logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_section
{
    FL_SECTION_VERSION,
    FL_SECTION_LITERALS,
    FL_SECTION_CODE,
};
typedef fl_byte fl_section;

// ---------- DECLARATIONS --------------
