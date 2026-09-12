/**
 * ---------- local_context.h --------------
 *
 * Declares local context logic.
 *
 * -----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "context/base_context.h"

// ------------- MACROS -----------------

// ...

// ---------- TYPE DEFINITIONS ----------

struct _fl_local_context
{
    fl_base_context *base;
    fl_scope *scope;
};
/**
 * @brief Describes generic local context. Used as an interface.
 */
typedef struct _fl_local_context fl_local_context;