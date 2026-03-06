#include "compiler/id_view.h"

#include "shared/memory.h"
#include "shared/str.h"

fl_void fl_id_view_node_init(fl_id_view_node **node, const fl_char *value)
{
    *node = fl_mem_alloc(sizeof(fl_id_view_node));
    (*node)->child = FL_NULL;
    (*node)->next = FL_NULL;
    (*node)->value = value;
}

fl_void fl_id_view_node_free(fl_id_view_node *node)
{
    if (!node)
        return;

    while (node)
    {
        fl_id_view_node_free(node->child);

        fl_id_view_node *temp = node->next;
        fl_mem_free(node);
        node = temp;
    }
}

const fl_id_view_node *fl_id_view_node_select(const fl_id_view_node *node, const fl_char *value)
{
    node = node->child;
    while (node)
    {
        if (fl_str_equal(node->value, value))
            return node;

        node = node->next;
    }

    return FL_NULL;
}

fl_bool fl_id_view_node_valid_id_chain(const fl_id_view_node *node, const fl_ast_node *id_chain)
{
    if (!node)
        return FL_FALSE;

    fl_ast_node *id = id_chain->child;

    while (node = fl_id_view_node_select(node, (const fl_char *)id->data))
    {
        id = id->next;
        if (!id)
            return FL_TRUE;
    }

    return FL_FALSE;
}

fl_void fl_id_view_node_include_id_chain(const fl_id_view_node *parent, const fl_ast_node *id_chain)
{
    fl_ast_node *id = id_chain->child;

    while (id)
    {
        parent = fl_id_view_node_append_id(parent->child, (const fl_char *)id->data);
        id = id->next;
    }
}

fl_id_view_node *fl_id_view_node_append_id(fl_id_view_node **node, const fl_char *value)
{
    if (*node == FL_NULL)
    {
        fl_id_view_node_init(node, value);
        return;
    }

    fl_id_view_node *ptr = *node;
    while (ptr->next)
    {
        // break if id already exists
        if (fl_str_equal(ptr->value, value))
            return FL_NULL;

        ptr = ptr->next;
    }

    fl_id_view_node_init(&ptr->next, value);

    return ptr->next;
}