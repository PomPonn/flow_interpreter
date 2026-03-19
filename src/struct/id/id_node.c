#include "id_node.h"

#include "shared/str.h"

fl_void fl_id_node_init(fl_id_node **node, const fl_id_entry *value)
{
    *node = fl_mem_alloc(sizeof(fl_id_node));
    (*node)->child = FL_NULL;
    (*node)->next = FL_NULL;
    (*node)->value = value;
}

fl_void fl_id_node_free(fl_id_node **node)
{
    fl_id_node *curr = *node;

    if (!curr)
        return;

    while (curr)
    {
        fl_id_node_free(&curr->child);

        fl_id_node *temp = curr->next;
        fl_mem_free(node);
        curr = temp;
    }

    *node = FL_NULL;
}

fl_id_node *fl_id_node_select(const fl_id_node *node, const fl_char *name)
{
    while (node)
    {
        if (fl_str_equal(node->value->name, name))
            return (fl_id_node *)node;

        node = node->next;
    }

    return FL_NULL;
}

fl_id_node *fl_id_node_push_sibling(fl_id_node **node, fl_id_entry *entry)
{
    if (!*node)
    {
        fl_id_node_init(node, entry);
        return *node;
    }

    fl_id_node *ptr = *node;
    while (ptr->next)
    {
        // break if id already exists
        if (fl_str_equal(ptr->value->name, entry->name))
            return FL_NULL;

        ptr = ptr->next;
    }

    fl_id_node_init(&ptr->next, entry);

    return ptr->next;
}

fl_id_node *fl_id_node_push_child(fl_id_node *node, fl_id_entry *entry)
{
    return fl_id_node_push_sibling(&node->child, entry);
}

fl_id_node *fl_id_node_seek(const fl_id_node *node, const fl_id_chain *chain)
{
    assert(chain->size != 0);

    if (!node)
        return FL_NULL;

    fl_size i = 0;
    while (node = fl_id_node_select(node, chain->data[i]))
    {
        i++;
        if (i == chain->size)
            return (fl_id_node *)node;
    }

    return FL_NULL;
}