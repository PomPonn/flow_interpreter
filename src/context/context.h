#pragma once

#include "shared/common.h"
#include "compiler/id_view.h"

// // byte code vector
// #define FL_VECTOR_NAME fl_byte_code
// #define FL_VECTOR_TYPE fl_byte
// #define FL_VECTOR_CAP 1024
// #include "generics/vector.h"
#include <compiler/byte_code.h>

struct _fl_context
{
    fl_size next_id_table_index;
    fl_byte_code byte_code;
    fl_id_view_node *id_view;
};
typedef struct _fl_context fl_context;

fl_void fl_context_init(fl_context *cnt);

fl_void fl_context_free(fl_context *cnt);

fl_size fl_context_next_id_table_index(fl_context *cnt);
