# TODO

## interpreter

- free op_vec memory after usage - implement operation_free
- opt. opportunity: implement container with unshifting without moving whole container data (used in ARGS_END case)

## compiler

- simulate assign instruction in some cases
- revise fn variants overloading

## ast generator

- revise token locations when sorting operators
- cleanup
- think again about single arg calls without parantheses
- AST generating optimalization
- AST operators sorting
- AST function definitions handling
- AST generation unused tokens handling
- AST types handling; AST type dimensions(?) handling

## other

- update specs
- handle ':' at the start of import string (maybe It's unnecessary?)
- revise c modules registration
- revise mod_importer implementation namings
- implement time measurement on unix systems
- think if warnings should be a thing
- handle other number bases (hex for instance)
- optimalize fl_std_io_put
- create proper error handling
