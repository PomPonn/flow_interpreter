/**
 * ---------- lambda.h --------------------
 *
 * Defines lambda instruction logic.
 *
 * ----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "vecs/byte_vec.h"
#include "vecs/cstr_vec.h"
#include "vecs/fn_variant_vec.h"
#include "flow/instr/instruction.h"
#include "flow/byte_segm.h"

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_value fl_instr_lambda(fl_value_vec args);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_value fl_instr_lambda(fl_value_vec args)
{
    // params
    assert(args.data[0].desc.type == FL_DATA_TYPE_ARRAY);
    // body ptr
    assert(args.data[1].desc.type == FL_DATA_TYPE_CODE_SEGM);

    fl_value_vec *head = args.data[0].data;
    fl_byte_segm *body = args.data[1].data;

    // hide body arg to avoid freeing it
    // we just move byte segment to fn structure
    args.data[1] = fl_value_empty();
    assert(*body->data == FL_INSTR_START_BLOCK);

    // interface fn traits ???

    fl_data_desc_vec params;
    fl_cstr_vec labels;
    fl_data_desc_vec_init_c(&params, head->size);
    fl_cstr_vec_init_c(&labels, head->size);

    for (fl_size i = 0; i < head->size; i++)
    {
        // temp

        assert(head->data[i].desc.type == FL_DATA_TYPE_STRING);

        fl_data_desc_vec_push(&params, fl_data_desc_create(FL_DATA_TYPE_ANY));
        fl_cstr_vec_push(&labels, fl_str_dup(head->data[i].data));
    }

    fl_fn_interface intf;
    fl_fn_interface_init(&intf, FL_DATA_TYPE_ANY, params);
    fl_fn_interface_set_labels(&intf, labels);

    fl_fn_variant variant;
    fl_fn_variant_init(&variant, FL_FALSE, intf, body);

    fl_fn_variant_vec *vars = fl_mem_alloc_t(fl_fn_variant_vec);
    fl_fn_variant_vec_init_with(vars, 1, (fl_fn_variant[]){variant});

    return fl_value_create(FL_DATA_TYPE_FN, vars);
}