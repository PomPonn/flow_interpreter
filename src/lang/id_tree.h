#pragma once

#include "shared/common.h"
#include "ast/ast.h"
#include "ast/id_chain.h"
#include "lang/id_entry.h"

struct _fl_id_tree_node
{
    fl_id_entry *value;
    struct _fl_id_tree_node *next;
    struct _fl_id_tree_node *child;
};
typedef struct _fl_id_tree_node fl_id_tree_node;

fl_void fl_id_tree_node_init(fl_id_tree_node **node, fl_id_entry *value);

const fl_id_tree_node *fl_id_tree_node_select(const fl_id_tree_node *node, const fl_char *name);

fl_id_entry *fl_id_tree_node_seek_id_chain(const fl_id_tree_node *node, const fl_id_chain *chain);

fl_id_tree_node *fl_id_tree_node_append_id(fl_id_tree_node **node, fl_id_entry *entry);

fl_void fl_id_tree_node_free(fl_id_tree_node *node);