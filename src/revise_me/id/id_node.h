#pragma once

#include "shared/common.h"
#include "flow/id/id_chain.h"
#include "flow/id/id_entry.h"

struct _fl_id_node
{
    const fl_id_entry *value;
    struct _fl_id_node *next;
    struct _fl_id_node *child;
};
typedef struct _fl_id_node fl_id_node;

fl_void fl_id_node_init(fl_id_node **node, const fl_id_entry *value);

fl_id_node *fl_id_node_select(const fl_id_node *node, const fl_char *name);

fl_id_node *fl_id_node_seek(const fl_id_node *node, const fl_id_chain *chain);

fl_id_node *fl_id_node_push_sibling(fl_id_node **node, fl_id_entry *entry);

fl_id_node *fl_id_node_push_child(fl_id_node *node, fl_id_entry *entry);

fl_void fl_id_node_free(fl_id_node **node);