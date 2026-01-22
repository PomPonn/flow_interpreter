## todo

- revise delimeters (it seems like some of them will be operators)
- fix optimalizer space handling
- decide whether ast_handle_operator function should return created node
- abstract syntax tree generation
    - expression & operators handling
    - unused literals handling
    - unused var declaration (in fl_ast_handle_statement) handling
- implement time measurement on unix systems
- (optimalization) consider use of inline functions

# optional

- \_fl_token_alias_arr and \_fl_token_type_arr is unused for now (consider removing it in the future)
- print errors to different destinations, eg. logs

# done

- write profiler
- new lines are not counted when creating comment and string tokens (tokenizer)
- whitespace tokenizer context member makes no sense
- make wrappers single character, not string, to apply some optimalization (since we leave universality)
- remove operators and keyword parsing from tokenizer (we have no predefined stuff)
- revise delimeters (reduce them to minimum)
- work on tokenizer - optimalization, string escape, etc.
- write text optimalizer (include whitespace, but reduce it to minimum)
- cleanup tokenizer
