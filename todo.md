# TODO

## active

- 'declare' instruction unused for now - use to it to declare variables (it doesn't work right now)
- fix memory leak in scope.h
- to opt.: simulated operations are called multiple times during compilation
- rework tokenizer main function interface (value return method)
- revise fl_token_alias
- inline imports?
- include compilation timestamp in byte code version section
- opt. opportunity: implement container with unshifting without moving whole container data
- optimalize importing - use already imported object if it exists
- what about operators ordering?
- handle ':' at the start of import string
- revise mod_importer implementation namings
- create forward type decls where possible

## freezed

- revise c modules registration
- AST optimalization
- AST operators sorting
- AST function definitions handling
- AST type dimensions(?) handling
- AST generation unused tokens handling
- think how to split source code into libraries optimally
- implement time measurement on unix systems
- think if warnings should be a thing
- handle other number bases (hex for instance)
- optimalize fl_std_io_put
- revise error handling
- AST types handling
