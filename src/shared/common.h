#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "shared/memory.h"
#include "shared/types.h"
#include <assert.h>

#define FL_NULL (void *)0
#define FL_FIND_FAIL -1

#define _fl_swap(type, a, b) \
    {                        \
        type temp = a;       \
        a = b;               \
        b = temp;            \
    }

#define FL_FLOW_MODULE __declspec(dllexport)
#define FL_HEADER_FN static inline