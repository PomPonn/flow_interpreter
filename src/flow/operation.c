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

// ---------- DECLARATIONS ----------

fl_void _fl_operation_adapt_args(fl_operation op, fl_value_vec *args);

// ---------- DEFINITIONS -----------

fl_fn_variant *fl_operation_prep_run(fl_operation op, fl_value_vec *args, fl_base_context *cnt)
{
    if (!fl_operation_is_callable(op))
        return FL_NULL;

    // TODO:
    // don't check values under refs twice

    fl_fn_variant *var = fl_fn_variant_vec_find_suit(op.vars, args);

    if (!var)
        assert(FL_FALSE);

    // adapt arguments (eg. discard references when needed)
    _fl_operation_adapt_args(op, args);

    // add base context as the first arg if META trait is set
    if (fl_bits_is_set(var->intf.traits, FL_FN_TRAITS_META))
    {
        fl_data_desc desc = fl_data_desc_create_traited(FL_DATA_TYPE_ANY, FL_DATA_TRAIT_VIEW);
        fl_value cnt_arg = fl_value_create_desc(desc, cnt);
        fl_value_vec_put_back(args, cnt_arg);
    }

    return var;
}

fl_void _fl_operation_adapt_args(fl_operation op, fl_value_vec *args)
{
    const fl_fn_intf *intf = fl_operation_get_intf_by_args(op, args);
    assert(intf);

    for (fl_size i = 0; i < args->size; i++)
    {
        // clean this up?

        if (!fl_bits_is_set(args->data[i].desc.traits, FL_DATA_TRAIT_REF))
            continue;

        if (fl_bits_is_set(intf->param_descs.data[i].traits, FL_DATA_TRAIT_REF))
            continue;

        // convert ref to simple value
        *(args->data + i) = ((fl_id_entry *)args->data[i].data)->value;
    }
}

const fl_fn_intf *fl_operation_get_intf_by_descs(
    fl_operation op,
    const fl_data_desc_vec *descs)
{
    if (!fl_operation_is_callable(op))
        return FL_NULL;
    else
    {
        fl_fn_variant *match = fl_fn_variant_vec_find_match(op.vars, descs);
        return match ? &match->intf : FL_NULL;
    }
}

const fl_fn_intf *fl_operation_get_intf_by_args(
    fl_operation op,
    const fl_value_vec *args)
{
    if (!fl_operation_is_callable(op))
        return FL_NULL;
    else
    {
        fl_fn_variant *match = fl_fn_variant_vec_find_suit(op.vars, args);
        return match ? &match->intf : FL_NULL;
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

    if (id && !fl_value_is_empty(id->value))
    {
        fl_value id_val = id->value;

        if (id_val.desc.type != FL_DATA_TYPE_FN)
            // error (?) - not-a-function id used as a callable
            assert(FL_FALSE);

        return fl_operation_from_fn_vars(id_val.data);
    }

    return fl_operation_unknown();
}
