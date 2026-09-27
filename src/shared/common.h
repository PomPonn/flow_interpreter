/**
 * ---------- common.h ----------------------
 *
 * Declares commonly used things across
 * the project.
 *
 * ------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/memory.h"
#include "shared/types.h"
#include "tools/profiler.h"
#include "templates/new.h"

#include <assert.h>

// ------------- MACROS -----------------

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define FL_NULL (void *)0
#define FL_FIND_FAIL -1
#define FL_FLOW_MODULE __declspec(dllexport)
#define FL_INLINE_FN static inline

#define FL_ID_CHAIN_MAX_SIZE 256

#define _fl_swap(type, a, b) \
    {                        \
        type temp = a;       \
        a = b;               \
        b = temp;            \
    }

#define _fl_max(a, b) (a > b ? a : b)

#define FL_FORWARD_STRUCT(struct_name) \
    struct _##struct_name;             \
    typedef struct _##struct_name struct_name;

// ---------- DEFINITIONS ----------

FL_NEW_MEM_FN_OF(fl_bool);
FL_NEW_MEM_FN_OF(fl_flow_bool);
FL_NEW_MEM_FN_OF(fl_flow_int);
FL_NEW_MEM_FN_OF(fl_flow_float);