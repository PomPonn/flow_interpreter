#include "scope.h"

#include "vecs/id_entry_vec.h"
#include "lang/id/scope_id_data.h"
#include "lang/id/id_chain.h"

static inline const fl_scope *_fl_scope_go_back(const fl_scope *scope, fl_uint back_offset);

fl_int_vec fl_scope_resolve_id(const fl_scope *scope, fl_id_chain *id_chain, fl_uint *outer_offset)
{
    fl_int_vec indexes;
    fl_int_vec_init(&indexes, id_chain->size);

    *outer_offset = 0;

    do
    {
        // search for id in current scope
        for (fl_size i = 0; i < id_chain->size; i++)
        {
            fl_int ind = fl_id_entry_vec_find(&scope->ids, id_chain->data[i]);
            if (ind == FL_FIND_FAIL)
                break;

            fl_int_vec_push(&indexes, ind);

            if (indexes.size == id_chain->size)
                return indexes;
            else
            {
                fl_id_entry *curr = scope->ids.data[ind];
                assert(curr->data_type == FL_DATA_TYPE_SCOPE);
                scope = &((fl_scope_id_data *)curr->data)->scope;
            }
        }

        // if didn't found in current scope, go to higher scope
        if (scope->outer)
        {
            scope = scope->outer;
            (*outer_offset)++;
        }
        else
            // there are no more scopes to search in, break the loop
            break;

    } while (indexes.size == 0);

    // didn't found, return empty vector
    fl_int_vec_free(&indexes);
    *outer_offset = 0;

    return indexes;
}

fl_id_entry *fl_scope_traverse(const fl_scope *scope, fl_int_vec inds, fl_uint back_offset)
{
    scope = _fl_scope_go_back(scope, back_offset);

    if (!scope)
        return FL_NULL;

    fl_id_entry_vec ids = scope->ids;

    for (fl_size i = 0; i < inds.size; i++)
    {
        int index = inds.data[i];

        if (index >= ids.size)
            return FL_NULL;

        if (i == inds.size - 1)
            return ids.data[index];
        else
        {
            assert(ids.data[index]->data_type == FL_DATA_TYPE_SCOPE);
            ids = ((fl_scope_id_data *)ids.data[index])->scope.ids;
        }
    }

    return FL_NULL;
}

static inline const fl_scope *_fl_scope_go_back(const fl_scope *scope, fl_uint back_offset)
{
    for (fl_uint i = 0; i < back_offset; i++)
        if (scope->outer)
            scope = scope->outer;
        else
            return FL_NULL;

    return scope;
}