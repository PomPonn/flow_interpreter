# TODO

## interpreter

- opt.: implement container with unshifting without moving whole container data (used in ARGS_END case (and fl_instr_info_vec_to_escapes?))

## compiler

- opt.: search for unused literals (got inlined) and discard them
- simulate assign instruction in some cases
- revise fn variants overloading

## ast generator

- opt. \_fl_ast_sort_operators_helper - redundant calls?
- tweak lambdas sorting order?
- AST generation unused tokens handling

## tokenizer

- revise fl_token_location

## other

- fix typing prediction system when checking lambda (current temp solution - set second arg type as ANY)
- handle in-string escapes better (fl_value_from_literal function)
- make some function struct instead of using fn_variant_vec directly?
- revise context scopes management
- add call_attached to local_context or scope? (rename this field?)
- make fl_id_entry use data_desc instead of just data_type? (decl_type fields)
- wrap string to x type conversion std functions
- handle messy data_desc & intfs handling.
- handle ':' at the start of import string (maybe It's unnecessary?)
- revise c modules registration
- revise mod_importer implementation namings
- implement time measurement on unix systems
- handle other number bases
- create proper error(/warning?) handling
- optimalize fl_std_io_put
