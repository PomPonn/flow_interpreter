#include "id_entry_vec.h"

#include "shared/str.h"
#include "struct/id/scope_id_data.h"

fl_int fl_id_entry_vec_find(const fl_id_entry_vec *vec, const fl_char *id_name)
{
    for (fl_int i = 0; i < vec->size; i++)
        if (fl_str_equal(vec->data[i]->name, id_name))
            return i;

    return FL_FIND_FAIL;
}

// fl_int_vec fl_id_entry_vec_seek(fl_id_entry_vec *vec, fl_id_chain *id_chain)
// {
//     fl_int_vec indexes;
//     fl_int_vec_init(&indexes, id_chain->size);

//     for (fl_size i = 0; i < id_chain->size; i++)
//     {
//         fl_int ind = fl_id_entry_vec_find(vec, id_chain->data[i]);
//         if (ind == FL_FIND_FAIL)
//             continue;

//         fl_int_vec_push(&indexes, ind);

//         if (indexes.size == id_chain->size)
//             return indexes;
//         else
//         {
//             assert(vec->data[ind]->type == FL_ID_ENTRY_VALUE_TYPE_SCOPE);
//             vec = &((fl_scope_id_data *)vec->data[ind])->inner_ids;
//         }
//     }

//     // didn't found, return empty vector
//     fl_int_vec_free(&indexes);
//     return indexes;
// }

// fl_id_entry *fl_id_entry_vec_traverse(fl_id_entry_vec *vec, fl_int_vec inds)
// {
//     for (fl_size i = 0; i < inds.size; i++)
//     {
//         int index = inds.data[i];

//         if (vec->size >= index)
//             return FL_NULL;

//         if (i == inds.size - 1)
//             return vec->data[index];
//         else
//         {
//             assert(vec->data[index]->type == FL_ID_ENTRY_VALUE_TYPE_SCOPE);
//             vec = &((fl_scope_id_data *)vec->data[index])->inner_ids;
//         }
//     }

//     return FL_NULL;
// }