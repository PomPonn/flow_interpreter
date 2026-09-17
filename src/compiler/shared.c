/**
 * ---------- shared.c ----------------
 *
 * Implements shared compiler logic.
 *
 * ------------------------------------
 */

#include "shared.h"

// ---------- DEPENDENCIES ----------

#include "ast/ast_node.h"
#include "vecs/byte_vec.h"
#include "flow/operation.h"
#include "context/cmp_context.h"
#include "flow/instr/instruction.h"

// ---------- DEFINITIONS -----------

fl_void fl_cmp_block_cnt_to_bytes(fl_byte_vec *bytes, const fl_cmp_context *cnt, const fl_cstr_vec *ignore_labels)
{
    fl_id_entry_vec ids = cnt->scope->ids;
    fl_size ignore_count = ignore_labels ? ignore_labels->size : 0;

    // sum id declarations mem block size and rest of this block size
    // ids.size * 2 for each FL_INSTR_DECL_ID tag and string termination '\0'
    fl_size total_size = (ids.size - ignore_count) * 2 + cnt->byte_code.size;

    fl_byte_vec_push(bytes, FL_INSTR_START_BLOCK);
    // push block byte size at the beginning and fill it later
    fl_size size_ind = bytes->size;
    fl_byte_vec_push_t(bytes, total_size, fl_size);

    // push id declarations at block start
    for (fl_size i = 0; i < ids.size; i++)
    {
        const fl_char *name = ids.data[i]->name;
        if (ignore_count && fl_cstr_vec_find(ignore_labels, name) != FL_FIND_FAIL)
            continue;

        total_size += fl_str_len(name);
        fl_byte_vec_push(bytes, FL_INSTR_DECL_ID);
        fl_byte_vec_push_str(bytes, ids.data[i]->name);
    }

    *(fl_size *)&bytes->data[size_ind] = total_size;

    fl_byte_vec_merge(bytes, &cnt->byte_code);
    fl_byte_vec_push(bytes, FL_INSTR_END_BLOCK);
}

fl_void fl_cmp_call_to_bytes(fl_ast_node *id_node, fl_operation op, fl_cmp_context *cnt)
{
    if (op.type == FL_OPERATION_INSTR)
        fl_byte_vec_push(&cnt->byte_code, op.instr);
    else if (fl_bits_is_any(op.type, FL_OPERATION_FN | FL_OPERATION_UNKNOWN))
    {
        // start CALL instruction
        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_CALL);
        // push information how to access id
        _fl_cmp_stamp_id_access(cnt, id_node);
    }
    else
        assert(FL_FALSE);

    // compile args
    _fl_cmp_compile_args(id_node->next, cnt);
}