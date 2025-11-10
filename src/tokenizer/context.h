#pragma once

#include "shared/common.h"

struct _fl_tok_context
{
    const fl_char **escapes;
    const fl_char **literal_wrappers;
    const fl_char **delimeters;
    const fl_char **operators;
    const fl_char **keywords;

    fl_uint escapes_num;
    fl_uint literal_wrappers_num;
    fl_uint delimeters_num;
    fl_uint operators_num;
    fl_uint keywords_num;

    const fl_char *comment_wrapper;
    const fl_char *ws;
    fl_char *escapes_peek;
};
typedef struct _fl_tok_context fl_tok_context;

void fl_tok_context_init(fl_tok_context *cnt,
                         const fl_char *comment_wrapper,
                         const fl_char *whitespace,
                         const fl_char *delims[], fl_uint delims_num,
                         const fl_char *operators[], fl_uint operators_num,
                         const fl_char *keywords[], fl_uint keywords_num,
                         const fl_char *literal_wrappers[], fl_uint literal_wrappers_num);

void fl_tok_context_free(fl_tok_context *context);