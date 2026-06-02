/**
 * ---------- common.h ----------------------
 *
 * Declares commonly used modules across
 * the project.
 *
 * ------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/memory.h"
#include "shared/types.h"

#include <assert.h>

// ------------- MACROS -----------------

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define FL_NULL (void *)0
#define FL_FIND_FAIL -1
#define FL_GLOBAL_MODULE_SPEC_CHAR ':'


#define FL_FLOW_MODULE __declspec(dllexport)

#define FL_HEADER_FN static inline


#define _fl_swap(type, a, b) \
    {                        \
        type temp = a;       \
        a = b;               \
        b = temp;            \
    }

#define FL_FORWARD_STRUCT(struct_name) \
    struct _##struct_name;             \
    typedef struct _##struct_name struct_name;
