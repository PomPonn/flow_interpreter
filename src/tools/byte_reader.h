/**
 * ---------- byte_reader.h -------------
 *
 * Declares byte reader logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "shared/str.h"
#include "vecs/byte_vec.h"
#include "flow/byte_segm.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_byte_reader
{
    const fl_byte_segm bytes;
    fl_size iterator;
};
typedef struct _fl_byte_reader fl_byte_reader;

// ---------- MACROS ----------

#define fl_byte_reader_next(reader_ptr, type) \
    (*(type *)fl_byte_reader_next_bytes(reader_ptr, sizeof(type)))

#define fl_byte_reader_skip(reader_ptr, type) \
    fl_byte_reader_skip_bytes(reader_ptr, sizeof(type))

// ---------- DECLARATIONS ----------

fl_byte_segm fl_byte_reader_extract_block_segm(fl_byte_reader *reader);

FL_SEALED_FN const fl_unknown *fl_byte_reader_next_bytes(fl_byte_reader *reader, fl_size size);
FL_SEALED_FN fl_void fl_byte_reader_skip_bytes(fl_byte_reader *reader, fl_size count);
FL_SEALED_FN fl_byte_reader fl_byte_reader_create(const fl_byte_vec bytes);
FL_SEALED_FN fl_byte_reader fl_byte_reader_create_off(const fl_byte_vec bytes, fl_size offset);
FL_SEALED_FN fl_byte_reader fl_byte_reader_create_segm(fl_byte_segm segm);
FL_SEALED_FN const fl_char *fl_byte_reader_next_str(fl_byte_reader *reader);
FL_SEALED_FN fl_bool fl_byte_reader_is_end(fl_byte_reader *reader);
FL_SEALED_FN const fl_byte *fl_byte_reader_get_ptr(fl_byte_reader *reader);

// ---------- HEADER-ONLY DEFINITIONS ----------

FL_SEALED_FN const fl_byte *fl_byte_reader_get_ptr(fl_byte_reader *reader)
{
    return reader->bytes.data + reader->iterator;
}

FL_SEALED_FN fl_byte_reader fl_byte_reader_create(const fl_byte_vec bytes)
{
    return (fl_byte_reader){
        .bytes = fl_byte_segm_create(bytes.data, bytes.size),
        .iterator = 0,
    };
}

FL_SEALED_FN fl_byte_reader fl_byte_reader_create_off(const fl_byte_vec bytes, fl_size offset)
{
    assert(bytes.size >= offset);

    return (fl_byte_reader){
        .bytes = fl_byte_segm_create(bytes.data, bytes.size),
        .iterator = offset,
    };
}

FL_SEALED_FN fl_byte_reader fl_byte_reader_create_segm(fl_byte_segm segm)
{
    return (fl_byte_reader){
        .bytes = segm,
        .iterator = 0,
    };
}

FL_SEALED_FN const fl_unknown *fl_byte_reader_next_bytes(fl_byte_reader *reader, fl_size size)
{
    assert(reader->iterator < reader->bytes.len);
    const fl_unknown *value = &reader->bytes.data[reader->iterator];
    reader->iterator += size;
    return value;
}

FL_SEALED_FN fl_void fl_byte_reader_skip_bytes(fl_byte_reader *reader, fl_size count)
{
    assert(reader->iterator + count <= reader->bytes.len);
    reader->iterator += count;
}

FL_SEALED_FN const fl_char *fl_byte_reader_next_str(fl_byte_reader *reader)
{
    const fl_char *start = (const fl_char *)(reader->bytes.data + reader->iterator);
    reader->iterator += fl_str_len(start) + 1;
    return start;
}

FL_SEALED_FN fl_bool fl_byte_reader_is_end(fl_byte_reader *reader)
{
    return reader->iterator >= reader->bytes.len;
}
