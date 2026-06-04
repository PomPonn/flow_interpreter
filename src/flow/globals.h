/**
 * ---------- globals.h --------------------
 *
 * Declares flow global context logic.
 *
 * -----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- DECLARATIONS --------------

fl_void fl_globals_init();

fl_void fl_globals_free();

fl_bool fl_globals_initialized();