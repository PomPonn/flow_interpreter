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
    // Header section. Contains general byte code info.
    FL_SECTION_MARK_HEADER,

    // Literals array section.
    FL_SECTION_MARK_LITERALS,

    // Actual code section.
    FL_SECTION_MARK_CODE,
};

/*
 * @brief Section mark.
 */
typedef fl_byte fl_section_mark;
