# TODO

## AST

- finish fl_ast_operator_data struct handling
- AST operators sorting
- AST function expression handling
- AST types handling (type dimensions?)
- think if we can use only single id ast node instead of id chain if there is no chaining
- AST generation unused tokens handling
- remember to implement convertion of literal info data field to appropiate type

## general

- implement time measurement on unix systems
- think if warnings should be a thing (they probably should, but for now I don't have any good use for them)
- test tokenizer
- handle other number bases (hex for instance)
- optimalize fl_std_io_put

# Q&A

- what to do with errors? How to report them when we free tokens and don't now the line?
- how to handle types? Are they predefined?
    - Yes, they are. a use of type is essentially a id chain and (optionally) dimensions qualifier
    - We won't now in advance if use of type should be next
- How to represent language structures in interpreter code?

# done

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
