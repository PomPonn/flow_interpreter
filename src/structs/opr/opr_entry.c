#include "opr_entry.h"

fl_void fl_opr_entry_init(
    fl_opr_entry **entry,
    const fl_char *name,
    fl_bool compile_time,
    fl_opr_ptr ptr,
    fl_data_type return_type,
    fl_call_param left,
    fl_call_param right)
{
    *entry = fl_mem_alloc(sizeof(fl_opr_entry));
    (*entry)->type = FL_ID_ENTRY_TYPE_OPR;
    (*entry)->compile_time = compile_time;
    (*entry)->name = name;
    (*entry)->pointer = ptr;
    (*entry)->return_type = return_type;
    (*entry)->left = left;
    (*entry)->right = right;
}

fl_void fl_opr_entry_free(fl_opr_entry *entry)
{
    fl_call_param_free(&entry->left);
    fl_call_param_free(&entry->right);
    fl_mem_free(entry);
}