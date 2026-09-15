/**
 * ---------- shared.c ----------------
 *
 * Implements shared compiler logic.
 *
 * ------------------------------------
 */

#include "shared.h"

// ---------- DEPENDENCIES ----------

#include "vecs/byte_vec.h"
#include "context/cmp_context.h"
#include "flow/instr/instruction.h"

// ---------- DEFINITIONS -----------

fl_void fl_cmp_block_cnt_to_bytes(fl_byte_vec *bytes, const fl_cmp_context *cnt)
{
    fl_id_entry_vec ids = cnt->scope->ids;

    // sum id declarations mem block size and rest of this block size
    // ids.size * 2 for each FL_INSTR_DECL_ID tag and string termination '\0'
    fl_size total_size = ids.size * 2 + cnt->byte_code.size;
    for (fl_size i = 0; i < ids.size; i++)
        total_size += fl_str_len(ids.data[i]->name);

    fl_byte_vec_push(bytes, FL_INSTR_START_BLOCK);

    // push block byte size at the beginning
    fl_byte_vec_push_t(bytes, total_size, fl_size);

    // push id declarations at block start
    for (fl_size i = 0; i < ids.size; i++)
    {
        fl_byte_vec_push(bytes, FL_INSTR_DECL_ID);
        fl_byte_vec_push_str(bytes, ids.data[i]->name);
    }

    fl_byte_vec_merge(bytes, &cnt->byte_code);
    fl_byte_vec_push(bytes, FL_INSTR_END_BLOCK);
}