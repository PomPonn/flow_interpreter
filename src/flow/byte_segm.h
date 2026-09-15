/**
 * ---------- byte_segm.h --------------------
 *
 * Declares readonly bytes segment logic.
 *
 * -------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "vecs/byte_vec.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_byte_segm
{
    fl_size len;
    const fl_byte *data;
};
typedef struct _fl_byte_segm fl_byte_segm;

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_byte_segm fl_byte_segm_create(const fl_byte *bytes, fl_size len);
FL_HEADER_FN fl_byte_segm fl_byte_segm_create_from_vec(const fl_byte_vec *vec);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_byte_segm fl_byte_segm_create(const fl_byte *bytes, fl_size len)
{
    return (fl_byte_segm){
        .data = bytes,
        .len = len,
    };
}

FL_HEADER_FN fl_byte_segm fl_byte_segm_create_from_vec(const fl_byte_vec *vec)
{
    return (fl_byte_segm){
        .data = vec->data,
        .len = vec->size,
    };
}