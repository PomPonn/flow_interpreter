/**
 * ---------- section_mark.h -----------------
 *
 * Declares byte code section mark logic.
 *
 * -------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_section_mark
{
    FL_SECTION_MARK_HEADER,
    FL_SECTION_MARK_LITERALS,
    FL_SECTION_MARK_CODE,
};
typedef fl_byte fl_section_mark;