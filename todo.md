# TODO

## active

- revise inline functions if they should be inline (not to big after expanding) - especially these header only
- opt.: can id entry names be made const ptrs?
- 'declare' instruction unused for now - use to it to declare variables (it doesn't work right now)
- to opt.: simulated operations are called multiple times during compilation
- rework tokenizer main function interface (value return method)
- inline imports? (preserve intruction?)
- opt. opportunity: implement container with unshifting without moving whole container data (for compiler ARGS_END)
- what about operators ordering?
- handle ':' at the start of import string

## freezed

- revise mod_importer implementation namings
- revise c modules registration
- AST generating optimalization
- AST operators sorting
- AST function definitions handling
- AST type dimensions(?) handling
- AST generation unused tokens handling
- implement time measurement on unix systems
- think if warnings should be a thing
- handle other number bases (hex for instance)
- optimalize fl_std_io_put
- revise error handling
- AST types handling
