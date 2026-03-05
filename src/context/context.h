#pragma once

#include "shared/common.h"

// byte code array
#define FL_VECTOR_NAME byte_code
#define FL_VECTOR_TYPE fl_byte
#define FL_VECTOR_CAP 1024
#include "generics/vector.h"

// id table array
#define FL_VECTOR_NAME id_table
#define FL_VECTOR_TYPE fl_char *
#define FL_VECTOR_CAP 16
#include "generics/vector.h"

struct _fl_context
{
    fl_id_table id_table;
    fl_byte_code byte_code;
};
typedef struct _fl_context fl_context;

fl_void fl_context_init(fl_context *cnt);

fl_void fl_context_free(fl_context *cnt);
