# TODO

## interpreter

- opt. opportunity: \_fl_intpr_indices_from_bytes - make indices readonly and don't alloc copy of them
- opt. opportunity: implement container with unshifting without moving whole container data (used in ARGS_END case)

## compiler

- opt.: search for unused literals (by inlining)?
- simulate assign instruction in some cases
- revise fn variants overloading

## ast generator

- tweak lambdas sorting order?
- test if second \_fl_ast_sort_operators_helper call in this function is necessary.
- revise token locations when sorting operators
- think again about single arg calls without parantheses
- AST types handling; AST type dimensions(?) handling
- AST generation unused tokens handling

## tokenizer

- revise fl_token_location

## other

- fix typing prediction system when checking lambda (temp solution - set second arg type as ANY)
- handle in-string escapes better (fl_value_from_literal function)
- make some function struct instead of fn_variant_vec?
- revise context scopes management
- add call_attached to local_context? (rename this field?)
- add const specifiers where possible
- make fl_id_entry use data_desc instead of just data_type? (decl_type fields)
- wrap std string to x type conversion functions
- fix messy data_desc & intfs handling.
- handle ':' at the start of import string (maybe It's unnecessary?)
- revise c modules registration
- revise mod_importer implementation namings
- implement time measurement on unix systems
- handle other number bases
- create proper error(/warning?) handling
- optimalize fl_std_io_put
