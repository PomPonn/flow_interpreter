#include "id_tree.h"

#include "shared/memory.h"
#include "shared/str.h"

fl_void fl_id_tree_node_init(fl_id_tree_node **node, fl_id_entry *value)
{
    *node = fl_mem_alloc(sizeof(fl_id_tree_node));
    (*node)->child = FL_NULL;
    (*node)->next = FL_NULL;
    (*node)->value = value;
}

fl_void fl_id_tree_node_free(fl_id_tree_node *node)
{
    if (!node)
        return;

    while (node)
    {
        fl_id_tree_node_free(node->child);
        fl_id_entry_free(node->value);

        fl_id_tree_node *temp = node->next;
        fl_mem_free(node);
        node = temp;
    }
}

const fl_id_tree_node *fl_id_tree_node_select(const fl_id_tree_node *node, const fl_char *name)
{
    node = node->child;
    while (node)
    {
        if (fl_str_equal(node->value->name, name))
            return node;

        node = node->next;
    }

    return FL_NULL;
}

fl_id_entry *fl_id_tree_node_seek_id_chain(const fl_id_tree_node *node, const fl_id_chain *chain)
{
    if (!node)
        return FL_NULL;

    for (fl_size i = 0; i < chain->size; i++)
        if (!(node = fl_id_tree_node_select(node, chain->data[i])))
            return FL_NULL;

    return node->value;
}

fl_id_tree_node *fl_id_tree_node_append_id(fl_id_tree_node **node, fl_id_entry *entry)
{
    if (*node == FL_NULL)
    {
        fl_id_tree_node_init(node, entry);
        return *node;
    }

    fl_id_tree_node *ptr = *node;
    while (ptr->next)
    {
        // break if id already exists
        if (fl_str_equal(ptr->value->name, entry->name))
            return FL_NULL;

        ptr = ptr->next;
    }

    fl_id_tree_node_init(&ptr->next, entry);

    return ptr->next;
}