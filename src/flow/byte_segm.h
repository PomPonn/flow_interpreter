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

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_byte_segm fl_byte_segm_create(const fl_byte *bytes, fl_size len)
{
    return (fl_byte_segm){
        .data = bytes,
        .len = len,
    };
}