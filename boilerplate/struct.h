/**
 * ---------- name.h ------------------
 *
 * Declares name logic.
 *
 * ------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_name
{
    // fields...
};
typedef struct _fl_name fl_name;

// ---------- DECLARATIONS ----------

FL_SEALED_FN fl_void fl_name_init(fl_name *name);
FL_SEALED_FN fl_void fl_name_free(fl_name *name);

// ---------- HEADER-ONLY DEFINITIONS ----------

FL_SEALED_FN fl_void fl_name_init(fl_name *name)
{
    // init...
}

FL_SEALED_FN fl_void fl_name_free(fl_name *name)
{
    // free...
}