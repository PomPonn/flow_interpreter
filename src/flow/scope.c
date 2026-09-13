/**
 * ---------- scope.c ----------------
 *
 * Implements scope logic.
 *
 * -----------------------------------
 */

#include "scope.h"

// ---------- DEPENDENCIES ----------

#include "flow/instr/instruction.h"
#include "flow/id/id_chain.h"
#include "vecs/byte_vec.h"

// ---------- DECLARATIONS ----------

inline const fl_scope *_fl_scope_go_back(const fl_scope *scope, fl_uint back_offset);

// ---------- DEFINITIONS -----------

fl_int_vec fl_scope_find_id_path(
    const fl_scope *scope,
    const fl_id_chain *id_chain,
    fl_uint *outer_offset)
{
    fl_int_vec indexes;
    fl_int_vec_init_c(&indexes, id_chain->size);

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
                fl_value id_value = scope->ids.data[ind]->value;

                if (id_value.desc.type != FL_DATA_TYPE_SCOPE)
                    goto __not_found_return;

                scope = (fl_scope *)id_value.data;
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

__not_found_return:
    // didn't found, return empty vector
    fl_int_vec_free(&indexes);
    *outer_offset = 0;

    return indexes;
}

fl_void fl_scope_to_bytes(const fl_scope *scope, fl_byte_vec *bytes)
{
    const fl_id_entry_vec ids = scope->ids;

    for (fl_size i = 0; i < ids.size; i++)
    {
        fl_byte_vec_push(bytes, FL_INSTR_DECL_ID);
        fl_value_to_bytes(ids.data[i]->value, bytes);
    }

    const fl_scope_vec *inners = fl_scope_get_inners(scope);
    for (fl_size i = 0; i < inners->size; i++)
    {
        fl_byte_vec_push(bytes, FL_INSTR_START_BLOCK);
        fl_scope_to_bytes(inners->data[i], bytes);
        fl_byte_vec_push(bytes, FL_INSTR_END_BLOCK);
    }
}

fl_id_entry *fl_scope_traverse(
    const fl_scope *scope,
    const fl_int_vec inds,
    fl_uint back_offset)
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
            fl_value id_value = scope->ids.data[index]->value;

            if (id_value.desc.type != FL_DATA_TYPE_SCOPE)
                return FL_NULL;

            ids = ((fl_scope *)id_value.data)->ids;
        }
    }

    return FL_NULL;
}

inline const fl_scope *_fl_scope_go_back(const fl_scope *scope, fl_uint back_offset)
{
    for (fl_uint i = 0; i < back_offset; i++)
        if (scope->outer)
            scope = scope->outer;
        else
            return FL_NULL;

    return scope;
}