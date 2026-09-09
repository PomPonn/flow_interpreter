/**
 * ---------- lambda.h --------------------
 *
 * Defines lambda instruction logic.
 *
 * ----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/id/id_entry.h"
#include "vecs/value_vec.h"
#include "vecs/byte_vec.h"
#include "vecs/fn_variant_vec.h"
#include "flow/id/id_entry.h"

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_value fl_instr_lambda(fl_value_vec args);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_value fl_instr_lambda(fl_value_vec args)
{
    assert(args.data[0].data_type == FL_DATA_TYPE_ARRAY);
    assert(args.data[1].data_type == FL_DATA_TYPE_SCOPE);

    fl_value_vec *head = args.data[0].data;
    fl_byte_vec *body = args.data[1].data;

    // param names ???
    // interface fn traits ???

    fl_param_vec params;
    fl_param_vec_init_c(&params, head->size);

    for (fl_size i = 0; i < head->size; i++)
    {
        // assert(head->data[i].data_type == FL_DATA_TYPE_REF);

        // fl_id_entry *id = head->data[i].data;
        fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    }

    fl_fn_interface intf;
    fl_fn_interface_init(&intf, FL_DATA_TYPE_ANY, params);

    fl_fn_variant variant;
    fl_fn_variant_init(&variant, FL_FALSE, intf, body);

    fl_fn_variant_vec *vars = fl_mem_alloc_t(fl_fn_variant_vec);
    fl_fn_variant_vec_init_with(vars, 1, (fl_fn_variant[]){variant});

    return fl_value_create(FL_DATA_TYPE_FN, vars);
}