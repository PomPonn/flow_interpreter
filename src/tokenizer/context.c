#include "context.h"

#include "shared/memory.h"

static fl_char *_tok_make_escapes_peek(fl_tok_context *cnt);
static void _tok_free_escapes_peek(fl_char *view);

void fl_tok_context_free(fl_tok_context *context)
{
    if (context == FL_NULL)
        return;

    _tok_free_escapes_peek(context->escapes_peek);
    fl_mem_free(context->escapes);
    context->escapes = FL_NULL;
    context->escapes_num = 0;
    context->escapes_peek = FL_NULL;
    context->literal_wrappers = FL_NULL;
    context->comment_wrapper = FL_NULL;
    context->ws = FL_NULL;
}

void fl_tok_context_init(fl_tok_context *cnt,
                         const fl_char *comment_wrapper,
                         const fl_char *whitespace,
                         const fl_char *delims[], fl_uint delims_num,
                         const fl_char *operators[], fl_uint operators_num,
                         const fl_char *keywords[], fl_uint keywords_num,
                         const fl_char *literal_wrappers[], fl_uint literal_wrappers_num)
{
    cnt->ws = whitespace;
    cnt->comment_wrapper = comment_wrapper;
    cnt->literal_wrappers = literal_wrappers;
    cnt->delimeters = delims;
    cnt->operators = operators;
    cnt->keywords = keywords;

    cnt->literal_wrappers_num = literal_wrappers_num;
    cnt->delimeters_num = delims_num;
    cnt->operators_num = operators_num;
    cnt->keywords_num = keywords_num;

    cnt->escapes_num = cnt->delimeters_num + cnt->operators_num + 1; // 1 for comment wrapper token
    cnt->escapes = fl_mem_alloc(cnt->escapes_num * sizeof(fl_char *));

    // fill escapes
    fl_size i = 0;
    cnt->escapes[i++] = comment_wrapper;
    for (fl_size j = 0; j < cnt->delimeters_num; j++)
    {
        cnt->escapes[i++] = delims[j];
    }
    for (fl_size j = 0; j < cnt->operators_num; j++)
    {
        cnt->escapes[i++] = operators[j];
    }

    cnt->escapes_peek = _tok_make_escapes_peek(cnt);
}

static fl_char *_tok_make_escapes_peek(fl_tok_context *cnt)
{
    fl_char *view = fl_mem_alloc(cnt->escapes_num + cnt->literal_wrappers_num + 1);

    for (fl_size i = 0; i < cnt->escapes_num; i++)
    {
        view[i] = *cnt->escapes[i];
    }

    for (fl_size i = 0; i < cnt->literal_wrappers_num; i++)
    {
        view[cnt->escapes_num + i] = *cnt->literal_wrappers[i];
    }

    view[cnt->escapes_num] = '\0';
    return view;
}

static void _tok_free_escapes_peek(fl_char *view)
{
    fl_mem_free(view);
}
