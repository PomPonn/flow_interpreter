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

// included only when profiler is on
#include "tools/profiler.h"

#include <assert.h>

// ------------- MACROS -----------------

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define FL_NULL (void *)0
#define FL_FIND_FAIL -1
#define FL_FLOW_MODULE __declspec(dllexport)
#define FL_HEADER_FN static inline

#define FL_ID_CHAIN_MAX_SIZE 256

#define _fl_swap(type, a, b) \
    {                        \
        type temp = a;       \
        a = b;               \
        b = temp;            \
    }

#define FL_FORWARD_STRUCT(struct_name) \
    struct _##struct_name;             \
    typedef struct _##struct_name struct_name;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_flow_int *fl_new_int(fl_flow_int init_value);

// ---------- DEFINITIONS ----------

FL_HEADER_FN fl_flow_int *fl_new_int(fl_flow_int init_value)
{
    fl_flow_int *f_int = fl_mem_alloc_t(fl_flow_int);
    *f_int = init_value;
    return f_int;
}