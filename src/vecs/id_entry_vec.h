/**
 * ---- id_entry.h ----
 *
 * Declares flow id entry vector logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

// #include "lang/id/id_chain.h"
// #include "vecs/int_vec.h"
#include "lang/id/id_entry.h"

#define FL_VEC_NAME fl_id_entry_vec
#define FL_VEC_TYPE fl_id_entry *
#define FL_VEC_CAP 16
#define FL_VEC_DESTRUCTOR(elem) fl_id_entry_free(&elem)
#include "templates/vector.h"

fl_int fl_id_entry_vec_find(const fl_id_entry_vec *vec, const fl_char *id_name);