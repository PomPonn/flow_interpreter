#pragma once

#include "lang/scope.h"

struct _fl_scope_id_data
{
    fl_scope scope;
};
typedef struct _fl_scope_id_data fl_scope_id_data;

FL_HEADER_FN fl_void fl_scope_id_data_init(fl_scope_id_data **data, const fl_scope *outer_scope)
{
    fl_scope_id_data *ptr = fl_mem_alloc(sizeof(fl_scope_id_data));
    fl_scope_init(&ptr->scope, outer_scope);

    *data = ptr;
}

FL_HEADER_FN fl_void fl_scope_id_data_free(fl_scope_id_data **data)
{
    fl_scope_free(&(*data)->scope);
    fl_mem_free(*data);

    *data = FL_NULL;
}