/**
 * ---------- while.h --------------------------
 *
 * Defines while ('@') instruction logic.
 *
 * ---------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "vecs/value_vec.h"
#include "context/local_context.h"
#include "interpreter/interpreter.h"
#include "flow/byte_segm.h"

// ---------- DECLARATIONS -------------

FL_INLINE_FN fl_value fl_instr_while(fl_value_vec args);

// ---------- DEFINITIONS --------------

FL_INLINE_FN fl_value fl_instr_while(fl_value_vec args)
{
    fl_value cnt_ref = args.data[0];
    fl_value left = args.data[1];
    fl_value right = args.data[2];

    assert(left.desc.type == FL_DATA_TYPE_CODE_SEGM);
    assert(right.desc.type == FL_DATA_TYPE_CODE_SEGM);

    fl_local_context *cnt = cnt_ref.data;
    fl_byte_segm *cond = left.data;
    fl_byte_segm *segm = right.data;

    while (FL_TRUE)
    {
        fl_value res = fl_intpr_eval(*cond, cnt);

        assert(!fl_value_is_empty(res));
        assert(res.desc.type == FL_DATA_TYPE_BOOL);

        fl_flow_bool *should_run = res.data;

        if (!*should_run)
            break;

        fl_value_free(&res);

        res = fl_intpr_eval(*segm, cnt);
        fl_value_free(&res);
    }

    return fl_value_empty();
}