#pragma once

#include "shared/common.h"
#include "lang/id_tree.h"
#include "lang/object.h"
#include "lang/function.h"
#include "compiler/byte_code.h"

struct _fl_context
{
    fl_id_tree_node *id_tree;
    fl_func_list func_list;
    fl_byte_code byte_code;
};
typedef struct _fl_context fl_context;

fl_void fl_context_init(fl_context *cnt);

fl_void fl_context_register_func(fl_context *cnt, fl_func_entry *entry);

fl_void fl_context_register_obj(fl_context *cnt, fl_object_entry *entry);

fl_void fl_context_free(fl_context *cnt);
