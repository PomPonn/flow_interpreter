/**
 * ---------- operation.c ----------------
 *
 * Implements operation logic.
 *
 * ---------------------------------------
 */

#include "operation.h"

// ---------- DEPENDENCIES ----------

#include "context/base_context.h"
#include "flow/id/id_chain.h"
#include "ast/ast_node.h"
#include "flow/scope.h"

// ---------- DEFINITIONS -----------

fl_value fl_operation_run(fl_operation op, fl_value_vec args, fl_base_context *cnt)
{
    if (!fl_operation_is_callable(op))
        return FL_VALUE_EMPTY;

    fl_fn_variant *var = fl_fn_variant_vec_find_suit(op.vars, args);

    if (!var)
        assert(FL_FALSE);

    // add context as the first arg if META trait is set
    if (fl_bitfield_is_set(var->interface.traits, FL_FN_TRAITS_META))
    {
        fl_value cnt_arg = fl_value_create(FL_DATA_TYPE_REF, cnt);
        fl_value_vec_put_back(&args, cnt_arg);
    }

    return fl_fn_variant_call(var, args);
}

const fl_fn_interface *fl_operation_get_interface(
    fl_operation op,
    const fl_data_desc_vec params)
{
    if (!fl_operation_is_callable(op))
        return FL_NULL;
    else
    {
        fl_fn_variant *match = fl_fn_variant_vec_find_match(op.vars, params);
        return match ? &match->interface : FL_NULL;
    }
}

fl_operation fl_operation_from_id_node(const fl_ast_node *id_node, const fl_scope *scope)
{
    fl_id_chain *id_chain = id_node->data;

    // check if it's instruction
    fl_instruction instr = id_chain->size == 1 ? fl_instruction_from_alias(id_chain->data[0]) : FL_INSTR_NONE;
    if (instr)
        return fl_operation_from_instr(instr);

    // try to find function id
    fl_id_entry *id = fl_scope_resolve_id(scope, id_chain);

    if (id)
    {
        fl_value id_val = id->value;

        if (id_val.data_type == FL_DATA_TYPE_FN)
            // error (?) - not-a-function id used as a callable
            assert(FL_FALSE);

        return fl_operation_from_fn_vars(id_val.data);
    }

    return fl_operation_unknown();
}