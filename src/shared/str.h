/**
 * ---------- str.h ------------------------
 *
 * Declares c-string manipulation logic.
 *
 * ----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- DECLARATIONS --------------

fl_bool fl_str_equal(const fl_char *str1, const fl_char *str2);
fl_bool fl_str_equal_n(const fl_char *str1, const fl_char *str2, const fl_size max_count);
fl_bool fl_str_in(const fl_char *str, const fl_char **arr, fl_size arr_size);
fl_bool fl_str_has(const fl_char *str, fl_char chr);
fl_bool fl_chr_is_digit(fl_char chr);
fl_bool fl_chr_is_any(fl_char chr, fl_char *pool);
fl_char *fl_str_dup(const fl_char *src);
fl_char *fl_str_find_any(const fl_char *str, const fl_char *chrs);
fl_size fl_str_len(const fl_char *str);
fl_bool fl_str_is_num(const fl_char *str, fl_bool *floating);
fl_char *fl_str_find(const fl_char *str, fl_char chr);
fl_long fl_str_index(const fl_char *str, fl_char chr);
