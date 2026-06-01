/**
 * ---------- globals.c -----------------
 *
 * Implements flow global context logic.
 *
 * -------------------------------------
 */

#include "globals.h"

// ---------- DEPENDENCIES ----------

// ...

// ---------- MACROS ----------------

// ...

// ---------- GLOBALS ---------------

fl_bool initialized = FL_FALSE;

// ---------- DECLARATIONS ----------

// ...

// ---------- DEFINITIONS -----------

fl_void fl_globals_init()
{
    fl_g_instrs_init();

    initialized = FL_TRUE;
}

fl_void fl_globals_free()
{
    if (!initialized)
        return;

    fl_g_instrs_free();
}

fl_bool fl_globals_initialized()
{
    return initialized;
}