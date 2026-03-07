#pragma once

#include "shared/common.h"
#include "compiler/ast.h"
#include "lang/id_entry.h"

struct _fl_id_tree_node
{
    fl_id_entry *value;
    struct _fl_id_tree_node *next;
    struct _fl_id_tree_node *child;
};
typedef struct _fl_id_tree_node fl_id_tree_node;

fl_void fl_id_tree_node_init(fl_id_tree_node **node, const fl_char *name);

const fl_id_tree_node *fl_id_tree_node_select(const fl_id_tree_node *node, const fl_char *name);

fl_id_tree_node *fl_id_tree_node_seek_id_chain(const fl_id_tree_node *node, const fl_ast_node *id_chain);

fl_void fl_id_tree_node_include_id_chain(fl_id_tree_node *parent, const fl_ast_node *id_chain);

fl_id_tree_node *fl_id_tree_node_append_id(fl_id_tree_node **node, const fl_char *name);

fl_void fl_id_tree_node_free(fl_id_tree_node *node);