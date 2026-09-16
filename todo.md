# TODO

## interpreter

- opt. opportunity: \_fl_intpr_indices_from_bytes - make indices readonly and don't alloc copy of them
- free op_vec memory after usage - implement operation_free (is it an error if sth stays unused on op_stack?)
- opt. opportunity: implement container with unshifting without moving whole container data (used in ARGS_END case)

## compiler

- simulate assign instruction in some cases
- revise fn variants overloading

## ast generator

- cleanup ast_tree.c
- revise token locations when sorting operators
- cleanup
- think again about single arg calls without parantheses
- AST generating optimalization
- AST operators sorting
- AST function definitions handling
- AST generation unused tokens handling
- AST types handling; AST type dimensions(?) handling

## tokenizer

- revise fl_token_location

## other

- add call_attached to local_context (rename this field?)
- free unused scopes faster?
- add const specifiers where possible
- make fl_id_entry use data_desc instead of just data_type? (decl_type fields)
- wrap std string to x type conversion functions
- implement values dereferencing
- fix messy data_desc & intfs handling.
- update specs
- handle ':' at the start of import string (maybe It's unnecessary?)
- revise c modules registration
- revise mod_importer implementation namings
- implement time measurement on unix systems
- think if warnings should be a thing
- handle other number bases (hex for instance)
- optimalize fl_std_io_put
- create proper error handling
