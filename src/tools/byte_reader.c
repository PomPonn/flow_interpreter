/**
 * ---------- byte_reader.c ----------------
 *
 * Implements byte reader logic.
 *
 * ----------------------------------
 */

#include "byte_reader.h"

// ---------- DEPENDENCIES ----------

#include "flow/instr/instruction.h"

// ---------- DEFINITIONS -----------

// add code segment pointer to the stack and skip whole block
fl_byte_segm fl_byte_reader_extract_block_segm(fl_byte_reader *reader)
{
    assert(*(fl_byte_reader_get_ptr(reader) - 1) == FL_INSTR_START_BLOCK);

    const fl_byte *block_start = fl_byte_reader_get_ptr(reader) - 1;
    const fl_size block_len = fl_byte_reader_next(reader, fl_size);
    // + 2 for BLOCK START & END tags
    fl_byte_segm segm = fl_byte_segm_create(block_start, block_len + sizeof(fl_size) + 2);

    // skip whole block
    fl_byte_reader_skip_bytes(reader, block_len + 1);
    return segm;
}