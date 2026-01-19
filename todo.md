## todo

- finish abstract syntax tree generation
    - expression & operators handling

# optional

- fl_token_alias_arr and fl_token_type_arr is unused for now (consider removing it in the future)
- print errors to different destinations, eg. logs
- optimalize tokens better in preprocesor module

# done

- whitespace tokenizer context member makes no sense
- make wrappers single character, not string, to apply some optimalization (since we leave universality)
- remove operators and keyword parsing from tokenizer (we have no predefined stuff)
- revise delimeters (reduce them to minimum)
- work on tokenizer - optimalization, string escape, etc.
- write text optimalizer (include whitespace, but reduce it to minimum)
- cleanup tokenizer
