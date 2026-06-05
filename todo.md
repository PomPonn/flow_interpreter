# TODO

## active

- how to handle simulating and inlining in compilation process?
- FL_DATA_TYPE_REF handling is quite messy
- think again of single arg calls without parantheses
- revise inline functions if they should be inline (not to big after expanding) - especially these header only
- opt.: can id entry names be made const ptrs?
- to opt.: simulated operations are called multiple times during compilation
- opt. opportunity: implement container with unshifting without moving whole container data (for compiler ARGS_END)
- what about operators ordering?
- handle ':' at the start of import string (maybe It's unnecessary?)
- AST generating optimalization
- AST operators sorting
- AST function definitions handling
- AST generation unused tokens handling
- AST types handling; AST type dimensions(?) handling

## distant

- revise mod_importer implementation namings
- revise c modules registration
- implement time measurement on unix systems
- think if warnings should be a thing
- handle other number bases (hex for instance)
- optimalize fl_std_io_put
- revise error handling
