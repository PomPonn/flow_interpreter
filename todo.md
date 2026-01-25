# todo

- abstract syntax tree generation
    - expression & operators handling
    - unused literals handling
    - unused var declaration (in fl_ast_handle_statement) handling
- implement time measurement on unix systems
- \_fl_token_alias_arr and \_fl_token_type_arr is unused for now (consider removing it in the future)

# done

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

# Q&A

- how to handle types? Are they predefined?
    - Yes, they are. a use of type is essentially a id chain and (optionally) dimensions qualifier
    - We won't now in advance if use of type should be next, so it has to be done dynamically in expression handling
- How to represent language structures in interpreter code?
