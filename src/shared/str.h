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

fl_size fl_str_len(const fl_char *str);
fl_bool fl_str_equal(const fl_char *str1, const fl_char *str2);
fl_bool fl_str_equal_n(const fl_char *str1, const fl_char *str2, const fl_size max_count);
fl_bool fl_str_in(const fl_char *str, const fl_char **arr, fl_size arr_size);
fl_bool fl_str_has(const fl_char *str, fl_char chr);
fl_bool fl_chr_is_digit(fl_char chr);
fl_bool fl_chr_is_any(fl_char chr, fl_char *pool);
fl_char *fl_str_dup(const fl_char *src);
fl_char *fl_str_find_any(const fl_char *str, const fl_char *chrs);
fl_bool fl_str_is_num(const fl_char *str, fl_bool *floating);
fl_char *fl_str_find(const fl_char *str, fl_char chr);
fl_size fl_str_index(const fl_char *str, fl_char chr);
fl_size fl_str_count_chr(const fl_char *str, fl_char chr);
fl_char *fl_str_as_printable(const fl_char *str, fl_char *buffer, fl_size buff_len);
fl_size fl_str_count_substrs(const fl_char *str, const fl_char *substr);
fl_char *fl_str_concat(fl_char *dest, const fl_char *src, fl_size dest_size);