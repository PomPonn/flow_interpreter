#include "predef.h"

#include "vecs/value_vec.h"
#include "flow/instr/internal/_all.h"
#include "flow/context.h"

fl_value fl_predef_call(const fl_predef *predef, fl_context *cnt, fl_value_vec args)
{
    switch (predef->instr)
    {
    case FL_INSTR_ADD:
        return fl_instr_add(args.data[0], args.data[1]);
    case FL_INSTR_ASSIGN:
        return fl_instr_assign(args.data[0], args.data[1]);
    case FL_INSTR_IMPORT:
        return fl_instr_import(cnt, args.data[0].data);
    default:
        assert(FL_FALSE);
        return FL_VALUE_EMPTY;
    }
}