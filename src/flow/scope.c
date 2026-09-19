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

// ---------- DEFINITIONS -----------

fl_void fl_scope_free(fl_scope **scope)
{
    fl_scope *scp = *scope;

    if (!scp)
        return;

    fl_id_entry_vec_free(&scp->ids);

    if (scp->inners)
    {
        fl_scope_vec_free(fl_scope_get_inners(scp));
        fl_mem_free(scp->inners);
    }

    if (scp->outer)
    {
        fl_scope_vec *inners = fl_scope_get_inners(scp->outer);
        fl_int ind = fl_scope_vec_find(inners, scp);
        assert(ind != FL_FIND_FAIL);

        inners->data[ind] = FL_NULL;
    }

    fl_mem_free(scp);
    *scope = FL_NULL;
}

fl_void fl_scope_init(fl_scope **scope, fl_scope *outer)
{
    fl_scope *scp = fl_mem_alloc_t(fl_scope);

    fl_id_entry_vec_init(&scp->ids);

    scp->outer = outer;
    if (scp->outer)
        fl_scope_add_inner(outer, scp);

    scp->inners = fl_mem_alloc_t(fl_scope_vec);
    fl_scope_vec_init(fl_scope_get_inners(scp));

    *scope = scp;
}

fl_id_entry *fl_scope_find_id(const fl_scope *scope, const fl_id_chain *id_chain)
{
    while (scope)
    {
        const fl_scope *curr_scope = scope;

        // Search for id in current scope
        for (fl_size i = 0; i < id_chain->size; i++)
        {
            const fl_id_entry_vec *ids = &curr_scope->ids;
            fl_int ind = fl_id_entry_vec_find(ids, id_chain->data[i]);

            // Id not found in this scope
            if (ind == FL_FIND_FAIL)
                break;

            if (i == id_chain->size - 1)
                // Id is found
                return ids->data[ind];

            fl_value id_value = ids->data[ind]->value;

            // Do we want to continue if id chain is so deceptive?
            if (id_value.desc.type != FL_DATA_TYPE_SCOPE)
                break;

            curr_scope = (fl_scope *)id_value.data;
        }

        // If didn't found id in current scope, go to outer scope
        scope = scope->outer;
    }

    return FL_NULL;
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