# TODO

- revise c modules registration
- comment code
- AST optimalization
- AST operators sorting
- AST function definitions handling
- AST type dimensions(?) handling
- AST generation unused tokens handling
- think how to split source code into libraries optimally
- implement time measurement on unix systems
- think if warnings should be a thing
- create unit tests
- handle other number bases (hex for instance)
- optimalize fl_std_io_put
- revise error handling
- types handling

# done

- templates optimalization (move some functions to specializations)
- revise in-context literal creation
- can I optimalize project building?
- change \*\_free functions to receive ptr to ptr (\*\*) so the pointer can be nulled by the function.
- make errors more universal
- fix AST operators order
- cleanup optimalizer
- cleanup tokenizer
- fix optimalizer space handling
- revise delimeters (it seems like some of them will be operators)
- write profiler
- new lines are not counted when creating comment and string tokens (tokenizer)
- whitespace tokenizer context member makes no sense
- make wrappers single character, not string, to apply some optimalization (since we leave universality)
- remove operators and keyword parsing from tokenizer (we have no predefined stuff)
- revise delimeters (reduce them to minimum)
- work on tokenizer - optimalization, string escape, etc.
- write text optimalizer (include whitespace, but reduce it to minimum)
- cleanup tokenizer
- (optimalization) consider use of inline functions
