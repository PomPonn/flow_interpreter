/**
 * ---- id_entry.h ----
 *
 * Declares flow id entry vector logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "struct/id/id_chain.h"
#include "struct/id/id_entry.h"
#include "struct/int_vec.h"

#define FL_VEC_NAME fl_id_entry_vec
#define FL_VEC_TYPE fl_id_entry *
#define FL_VEC_CAP 16
#include "templates/vector.h"

fl_int fl_id_entry_vec_find(fl_id_entry_vec *vec, const fl_char *id_name);

fl_int_vec fl_id_entry_vec_seek(fl_id_entry_vec *vec, fl_id_chain *id_chain);

fl_id_entry *fl_id_entry_vec_traverse(fl_id_entry_vec *vec, fl_int_vec inds);