#pragma once

#include "shared/common.h"
#include "compiler/ast.h"

struct _fl_id_view_node
{
    const fl_char *value;
    struct _fl_id_view_node *next;
    struct _fl_id_view_node *child;
};
typedef struct _fl_id_view_node fl_id_view_node;

fl_void fl_id_view_node_init(fl_id_view_node **node, const fl_char *value);

const fl_id_view_node *fl_id_view_node_select(const fl_id_view_node *node, const fl_char *value);

fl_bool fl_id_view_node_valid_id_chain(const fl_id_view_node *node, const fl_ast_node *id_chain);

fl_void fl_id_view_node_include_id_chain(fl_id_view_node *parent, const fl_ast_node *id_chain);

fl_id_view_node *fl_id_view_node_append_id(fl_id_view_node **node, const fl_char *value);

fl_void fl_id_view_node_free(fl_id_view_node *node);