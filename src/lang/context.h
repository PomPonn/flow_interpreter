#pragma once

#include "shared/common.h"
#include "lang/id_tree.h"
#include "compiler/byte_code.h"
#include "lang/function.h"

struct _fl_context
{
    fl_size next_id_table_index;
    fl_func_table func_table;
    fl_id_tree_node *id_tree;
    fl_byte_code byte_code;
};
typedef struct _fl_context fl_context;

fl_void fl_context_init(fl_context *cnt);

fl_void fl_context_free(fl_context *cnt);

fl_size fl_context_next_id_table_index(fl_context *cnt);
