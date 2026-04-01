#pragma once

#include "lang/value.h"
#include "lang/id_entry.h"
#include "lang/context.h"

#define _FL_V_INT(v) *(fl_lang_type_int *)v.data
#define _FL_V_FLOAT(v) *(fl_lang_type_int *)v.data

/**
 * @brief Represents byte code instructions.
 */
enum _fl_instruction
{
    // TODO: comment this when finished

    // predefined instructions (predefinitions)
    FL_INSTR_IMPORT,
    FL_INSTR_ASSIGN,
    FL_INSTR_ADD,
    FL_INSTR_SUB,
    FL_INSTR_DIV,
    FL_INSTR_MUL,
    FL_INSTR_MOD,
    FL_INSTR_POW,
    // ...
    FL_INSTR_CALL,
    FL_INSTR_GET_ID,
    FL_INSTR_GET_LITERAL,
    FL_INSTR_NEW,
    // use questioned...
    FL_INSTR_STARTBLOCK,
    FL_INSTR_ENDBLOCK,
    FL_INSTR_STARTOP,
    FL_INSTR_ENDOP,
};

FL_HEADER_FN fl_value fl_instr_add(fl_value left, fl_value right)
{
    // unsave conversion
    // TODO: implement type conversion system

    if (left.data_type == FL_DATA_TYPE_INT)
    {
        fl_lang_type_int *res = fl_mem_alloc(sizeof(fl_lang_type_int));
        *res = _FL_V_INT(left) + _FL_V_INT(right);
        return fl_value_create(FL_DATA_TYPE_INT, res);
    }
    else if (left.data_type == FL_DATA_TYPE_FLOAT)
    {
        fl_lang_type_float *res = fl_mem_alloc(sizeof(fl_lang_type_float));
        *res = _FL_V_FLOAT(left) + _FL_V_FLOAT(right);
        return fl_value_create(FL_DATA_TYPE_FLOAT, res);
    }
    else
    {
        assert(FL_FALSE);
        return FL_VALUE_EMPTY;
    }
}

FL_HEADER_FN fl_value fl_instr_assign(fl_value id_ref, fl_value what)
{
    // WARNING: 'what' value is moved, not copied
    // it is probably unwanted behaviour

    fl_id_entry *id = (fl_id_entry *)id_ref.data;

    // invalid data type
    if (!fl_data_type_has(id->decl_type, what.data_type))
        assert(FL_FALSE);

    fl_value_free(&id->value);
    id->value = what;

    return what;
}

FL_HEADER_FN fl_scope *fl_instr_import(fl_context *cnt, fl_value import_str)
{
    fl_scope *imported = fl_mem_alloc(sizeof(imported));

    assert(FL_FALSE);

    return imported;
}